import pytest
import subprocess

@pytest.mark.slowtest
def test_x11perf():
    openssl_cmd = ["openssl", "speed"]
    proc = subprocess.Popen(openssl_cmd)
    proc.wait()
    assert proc.returncode == 0

##################################################

TMUX=`whereis tmux`
sessions=5
windows=4

pgrep tmux > /dev/null 2>&1 && echo "You have already started tmux." && exit;

echo -n "Start tmux server: "
$TMUX start-server && echo "OK"

# sessions
for t in `jot $sessions 1 $sessions`;
do
	$TMUX new-session -d -s session-$t;
	# windows
	for w in `jot $windows 1 $windows`;
	do
		$TMUX new-window -n window-$w;
	done
done

echo -n "Sessions creation: "
nums=`$TMUX list-sessions | wc -l`
if [ $nums -eq $sessions ];
then
	echo "OK"
else
	echo "FAIL: Should be $sessions instead $nums"
fi

num=`jot -r 1 1 $sessions`
numw=`$TMUX list-windows -t session-$num | wc -l`
echo -n "Windows creation: ";
if [ $numw -eq $windows ];
then
	echo "OK";
else
	echo "FAIL: Should be $windows instead $numw"
fi

num=`jot -r 1 1 $sessions`
echo -n "Session rename: "
$TMUX rename-session -t session-$num session-rename > /dev/null 2>&1;
$TMUX list-sessions | grep session-rename > /dev/null 2>&1 && echo "OK" || echo FAIL""
$TMUX rename-session -t session-rename session-$num > /dev/null 2>&1;

num=`jot -r 1 1 $sessions`
echo "Session kill: "
$TMUX list-sessions | grep session-$num  > /dev/null 2>&1 && echo "\tSession $num running"
$TMUX kill-session -t session-$num > /dev/null 2>&1 && echo "\tKill session $num: OK"
$TMUX list-sessions | grep session-$num && echo "\tKilled FAIL" || echo "\tKilled OK"

#TODO: $TMUX kill-window -t session-$num

echo -n "Window options: "
$TMUX set-window-option clock-mode-style 24 > /dev/null 2>&1 &&
$TMUX show-window-options | grep clock-mode-style > /dev/null 2>&1 &&
echo "OK" || echo "FAIL"

# panes
#$TMUX list-panes

buffer=$(mktemp -t bufferXXXXXXXXXX);
head -100 /dev/urandom | uuencode - > $buffer
echo -n "Buffer load: "
$TMUX load-buffer $buffer > /dev/null 2>&1 &&
$TMUX list-buffers > /dev/null 2>&1 && echo "OK"
echo -n "Buffer compare: "
$TMUX show-buffer -b 0 | diff - $buffer > /dev/null 2>&1 && echo "OK" || echo "FAIL"
echo -n "Buffer remove: "
$TMUX delete-buffer -b 0 > /dev/null 2>&1;
if [ `$TMUX list-buffers | wc -l` -eq 0 ];
then
	echo "OK"
else
	echo "FAIL"
fi

#TODO: $TMUX save-buffer;
#TODO: $TMUX set-buffer;
#TODO: $TMUX paste-buffer;
rm -f $buffer

# clients
#$TMUX attach-client
#$TMUX suspend-client
#$TMUX switch-client
#$TMUX detach-client
#$TMUX list-clients

# messages
##$TMUX display-message mimimi
##$TMUX show-messages | grep mimimi > /dev/null 2>&1;
##$TMUX server-info
##$TMUX clock-mode

echo -n "Set environment variable: "
$TMUX set-environment REGRESS 1 > /dev/null 2>&1 && echo "OK" || echo "FAIL"
echo -n "Validate environment variable: "
$TMUX show-environment | grep "REGRESS=1" > /dev/null 2>&1 && echo "OK" || echo "FAIL"

echo -n "List of commands: "
$TMUX list-commands | grep server-info > /dev/null 2>&1 && echo "OK" || echo "FAIL"

echo -n "Set option: "
$TMUX set-option -s history-limit 1000 > /dev/null 2>&1 && echo "OK" || echo "FAIL"
echo -n "Validate option: "
$TMUX show-options | grep history-limit > /dev/null 2>&1 && echo "OK" || echo "FAIL"

echo -n "Terminate tmux server: "
$TMUX kill-server && echo "OK" || echo "FAIL"

##################################################
# stress test

TMUX=`whereis tmux`
num_windows=10
num_sessions=10

pgrep tmux > /dev/null 2>&1 && echo "You have already started tmux." && exit;

$TMUX start-server && echo "Successfull start tmux server"

s=`jot -r 1 1 $num_sessions`
echo "random s" $s
while true;
do
	for t in `jot $s 1 $s`;
	do
		$TMUX new-session -d > /dev/null 2>&1;
		#w=`jot -r 1 1 $num_windows`
		#for w in `jot $w 1 $w`;
		#do
			#$TMUX new-window -P -n window-$w > /dev/null 2>&1;
		#done
	done

	sessions_run=`$TMUX list-sessions | wc -l`
	sessions_kill=`jot -r 1 1 $sessions_run`
	sessions_die=1
	for t in `$TMUX list-sessions | sed 's;^\([0-9]*\):.*;\1;' | tr "\n" " "`;
	do
		$TMUX kill-session -t $t > /dev/null 2>&1 && $(($session_die++))
		if [ $session_die -eq $session_kill ];
		then
			break;
		fi
	done
	#while [ $session_die -eq $session_kill ];
	#do
		#n=`$TMUX list-sessions | sed 's;^\([0-9]*\):.*;\1;' | head -1`
		#$TMUX kill-session $n;
		#$((session_die++));
	#done
done

$TMUX kill-server && echo "Successfull terminate tmux server"
