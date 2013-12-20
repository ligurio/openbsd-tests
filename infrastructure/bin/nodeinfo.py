import sys, os, pprint
import yaml

# TODO: usbdevs(8), pcidump(8), acpidump(8)

def get_cpu_topology():

	logcpus = os.popen("sysctl -n hw.ncpu").readline().strip()

	return {
			"hw_cpus_cores":	logcpus,
		}

def get_meminfo():
	ret = {}
	ret['hw_mem_size'] = os.popen("sysctl -n hw.physmem").readline().strip()
	ret['hw_swap_size'] = 0

	return ret

def get_cpufreq():

	val = 200
	return {
			"hw_cpus_mhz":	int(val) / 1000
		}

def get_cpuinfo():
	ret = {}
	ret['hw_cpus_vendor'] = os.popen("sysctl -n machdep.cpuvendor").readline().strip()
	ret['hw_cpus_type'] = os.popen("sysctl -n hw.model").readline().strip()
	ret['hw_cpus_mhz'] = os.popen("sysctl -n hw.cpuspeed").readline().strip()

	return ret

def get_platforminfo():
	ret = {}
	ret["os"] = os.popen("sysctl -n kern.ostype").readline().strip()
	ret["os_version"] = os.popen("sysctl -n kern.osrelease").readline().strip()
	ret["kernel"] = os.popen("sysctl -n kern.version").readline().strip()
	ret["hostname"] = os.popen("sysctl -n kern.hostname").readline().strip()
	ret["arch"] = os.popen("sysctl -n hw.machine").readline().strip()

	man = os.popen("sysctl -n hw.vendor").readline().strip()
	product = os.popen("sysctl -n hw.product").readline().strip()
	version = os.popen("sysctl -n hw.version").readline().strip()
	ret['hw_model'] = man + " " + version + " " + product

	ret['hw_uuid'] = os.popen("sysctl -n hw.uuid").readline().strip()
	ret['hw_serial_num'] = os.popen("sysctl -n hw.serialno").readline().strip()
	ret['hw_manufacturer'] = man

	return ret

#def get_dmesg():
#	ret = {}
#	f = open('/var/run/dmesg.boot', 'r')
#	ret['hw_dmesg'] = f.readlines()
#	f.close()
#
#	return ret

def get_nodeinfo():
	ret = get_platforminfo()
	ret.update(get_cpuinfo())
	ret.update(get_cpufreq())
	ret.update(get_cpu_topology())
	ret.update(get_meminfo())
	#ret.update(get_dmesg())
	return ret

if __name__ == "__main__":
	pprint.pprint(get_nodeinfo())

	#f = open('node_info.yaml', 'w'),
	#s = yaml.safe_dump({'environments': get_nodeinfo()}, f)
	#f.close()
