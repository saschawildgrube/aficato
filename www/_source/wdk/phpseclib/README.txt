phpseclib 2.0.55 (MIT)
https://github.com/phpseclib/phpseclib/tree/2.0.55

Unmodified subset: phpseclib/Crypt/Base.php and phpseclib/Crypt/Rijndael.php.
LICENSE is included. No Composer installation or autoloader is needed.
WDK uses only CBC, explicit keys and IVs, and the internal PHP engine.
Other Base.php features (such as setPassword) may require additional upstream files.

Deploy this directory together with wdk_encryption-symmetric.inc.
Rijndael-128/192/256 work without mcrypt on PHP 7.4.33.
Other historical algorithm names still require the mcrypt extension.

The existing quality/tests/wdk-encryption/test_encryption-symmetric.php test
checks round trips on PHP 7.4 and additionally compares both directions and
exact ciphertext against mcrypt when that extension is installed.
Run on both target PHP 7.4.33 and a legacy mcrypt host before rollout.
The cross-check is explicitly skipped on hosts without mcrypt.
Legacy SHA-1 key derivation, zero padding, X marker, leading-NUL trimming and
StringSection UTF-8 semantics are deliberately retained for compatibility.