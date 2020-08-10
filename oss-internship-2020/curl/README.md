# Curl Sandboxed

This library is a sandboxed version of the original [curl](https://curl.haxx.se/libcurl/c/) C library, implemented using sandboxed-api.

## Supported methods

The library currently supports curl's [*Easy interface*](https://curl.haxx.se/libcurl/c/libcurl-easy.html). According to curl's website:

> The easy interface is a synchronous, efficient, quickly used and... yes, easy interface for file transfers. 
> Numerous applications have been built using this.

#### Variadic methods

Variadic methods are currently not supported by sandboxed-api. Because of this, the sandboxed header `custom_curl.h` wraps the curl library and explicitly defines the variadic methods.

For example, instead of using `curl_easy_setopt`, one of these methods can be used: `curl_easy_setopt_ptr`, `curl_easy_setopt_long` or `curl_easy_setopt_curl_off_t`.

#### Function pointers

The functions whose pointers will be passed to the library's methods (*callbacks*) can't be implemented in the files making use of the library, but must be in other files. These files must be compiled together with the library, and this is done by adding their absolute path to the `CURL_SAPI_CALLBACKS` variable. 

The pointers can then be obtained using an `RPCChannel` object, as shown in `example2.cc`.

## Examples

The `examples` directory contains the sandboxed versions of example source codes taken from [this](https://curl.haxx.se/libcurl/c/example.html) page on curl's website.

The `callbacks.h` and `callbacks.cc` files implemenent all the callbacks used by the examples.