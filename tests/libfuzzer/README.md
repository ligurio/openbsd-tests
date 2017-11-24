## libfuzzer tests

- https://cvsweb.openbsd.org/cgi-bin/cvsweb/src/lib/
- https://llvm.org/docs/LibFuzzer.html
- https://github.com/Dor1s/libfuzzer-workshop/tree/master/lessons/08#generate-coverage-report
- https://github.com/openssl/openssl/tree/master/fuzz


```
#include <stddef.h>
#include <stdint.h>

#include "base/sha1.h"

// Entry point for LibFuzzer.
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  unsigned char sha1[base::kSHA1Length] = {};
  base::SHA1HashBytes(data, size, sha1);
  return 0;
}
```
