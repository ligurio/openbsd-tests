### Run tests

- Install PyTest extension, it allows to produce HTML-based reports:
```
pip install pytest-html
```
- Checkout latest version of testsuite:
```
git clone https://github.com/ligurio/openbsd-tests/
```
- And finally execute tests:
```
pytest --self-contained-html --html=report-`date +%Y-%m-%d`.html --junit-xml=report-`date +%Y-%m-%d`.xml
```
