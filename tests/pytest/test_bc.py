from hypothesis import given
from hypothesis.strategies import text
from lark import Lark
import hypothesis.extra.lark
import unittest


class TestEncoding(unittest.TestCase):
    @given(text())
    def test_decode_inverts_encode(self, s):
        # self.assertEqual(decode(encode(s)), s)
        print(s)
        m = hypothesis.extra.lark.from_lark(
            "python3.lark", start='hello_world')
        print(m)


if __name__ == '__main__':
    unittest.main()
