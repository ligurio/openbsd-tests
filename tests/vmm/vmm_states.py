#!/usr/bin/python

# https://wiki.libvirt.org/page/VM_lifecycle
# https://github.com/spotify/python-graphwalker
# graphwalker --reporter=Print:output=stderr --stopcond=Coverage:edges=100 vmm.tgf vmm_states.VMM
# graphwalker --reporter=Print:output=stderr --stopcond=Seconds:1 --reporter=Cartographer:dotpath=.,imgpath=. vmm.tgf vmm_states.VMM
# graphwalker --reporter=Print:output=stderr --stopcond=CountSteps:steps=100 vmm.tgf vmm_states.VMM

class VMM(object):

    def __init__(self):
        self.calls = []

    def __getattr__(self, k):
        def f(*al, **kw):
            self.calls.append((k, al, kw))
        f.__name__ = k
        return f

    def teardown(self, a):
        """
        teardown is called the same way as setup, at the end.
        """

        print("teardown")
        pass

    @classmethod
    def setup(self, a):
        """
        setup is called at the start of the test session with a dictionary containing
        the other instances involved in the test: the reporter, the model, and so on.
        Notably, if you want to save attachments from the test methods, you should use
        the reporter instance here.
        """

        print("setup")
        pass

    def step_begin(self, a):
        """
        step_begin is called before each step with the step definition. The step
        definition is an iterable where the first is the id and the second the name of
        the step.
        """

        print("step_begin")
        pass

    def step_end(self, a, b):
        """
        step_end is called before after each step like step_begin, but with the
        addition of a failure, usually None. If the test failed, or there was some
        other exception, step_end is called with that exception, typically an
        AssertionError. The step_end method can permit the testing to continue by
        returning the exact string "RECOVER".
        """

        print("step_end")
        pass

    def e_once(self):

        print("e_once")
        pass

    def e_stop(self):

        print("e_stop")
        pass

    def e_start(self):

        print("e_start")
        pass

    def e_pause(self):

        print("e_pause")
        pass

    def e_unpause(self):

        print("e_unpause")
        pass
