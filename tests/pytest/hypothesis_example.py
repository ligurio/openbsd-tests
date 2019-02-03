import hypothesis.extra.lark
from lark import Lark

h = hypothesis.extra.lark.from_lark(Lark.open('python3.lark', start='file_input'))

h.example()
