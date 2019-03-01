RUNTEST(1) - General Commands Manual

# NAME

**runtest** - is a program for running OpenBSD regression tests.

# SYNOPSIS

**runtest**
\[**-h**]
\[**-d**&nbsp;*directory*]
\[**-f**&nbsp;*filter*]
\[**-l**&nbsp;*list*]
\[**-t**&nbsp;*timeout*]
\[**-o**&nbsp;*report*]
\[**-v**]

# DESCRIPTION

The
**runtest**
executes regression tests.
The options are as follows:

**-d**

> Directory where
> **runtest**
> will search tests.

**-f**

> Specify regular expression to filtering tests.

**-l**

> Lists found tests.

**-t**

> Timeout.

**-o**

> Filename where
> **runtest**
> will output TAP-compliant test results.

# STANDARDS

M. G. Schwern, A. Lester, A. Armstrong,
*TAP 13 - The Test Anything Protocol v13*,
2003-2007.

# EXIT STATUS

The **runtest** utility exits&#160;0 on success, and&#160;&gt;0 if an error occurs.

# SEE ALSO

regex(3)

# AUTHORS

The
**runtest**
utility was written by
Sergey Bronnikov

Debian - December 28, 2018
