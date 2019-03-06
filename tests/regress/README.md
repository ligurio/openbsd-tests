### Usage

```
$ pkg_add py-test
$ pip install pytest pytest-html
$ pytest --self-contained-html --html=report-`date +%Y-%m-%d`.html \
		--junit-xml=report-`date +%Y-%m-%d`.xml
```
