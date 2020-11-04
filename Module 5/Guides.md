## Guide Documentation on using IFTT functions to send webhooks to IFTT platform.

We performed a simple HTTP POST using the 'easy interface' of programming with libcurl

### 1. API Reference & Quickstart guide:
  * **#include <curl/curl.h>** header file
  * The easy interface is a synchronous, efficient, quickly used and easy interface for file transfers. Numerous applications have been built using this.  
  * It uses standard HTTP response codes, authentication, and verbs.
  * When using libcurl you init your easy-session and get a handle, which you use as input to the following interface functions you use:
    * **curl\_easy\_init()**

### 2. Authentication:
  * Uses API keys to authenticate requests.
  * Most protocols support that you specify the name and password in the URL itself. libcurl will detect this and use them accordingly.
  * libcurl also provides options to set various passwords. The user name and password as shown embedded in the URL can instead get set with the CURLOPT_USERPWD option.   
    The argument passed to libcurl should be a char * to a string in the format "user:password". In a manner like this:
    * **curl\_easy\_setopt(easyhandle, CURLOPT_USERPWD, "myname:thesecret");**
  * Another case where name and password might be needed at times, is for those users who need to authenticate themselves to a proxy they use.   
    libcurl offers another option for this, the CURLOPT\_PROXYUSERPWD. It is used quite similar to the CURLOPT_USERPWD option like this:
    * **curl\_easy\_setopt(easyhandle, CURLOPT_PROXYUSERPWD, "myname:thesecret");**

### 3. HTTP POSTing:
  * We have made use of **void http_post(char *Url,char *data)**, which takes up two parameters:
    * char *Url: The URL that is about to receive our POST.
    * char *data: Specify the POST data.
  * One can set properties and options for this handle using **curl_easy_setopt**. They control how the subsequent transfer or transfers will be made.   
    Options remain set in the handle until set again to something different.
  * We can set URL for POSTing using: **curl_easy_setopt(curl, CURLOPT_URL, "YOUR_URL");**
  * We can send data using: **curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);**
  * We can also pass our list of custom made headers: **curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);**
  * To perform the request, we have used: **res = curl_easy_perform(curl);**

### 4. Error Handling:
  * Raises error if **curl\_easy\_perform()** fails:
    * **fprintf(stderr, "curl\_easy\_perform() failed: %s\n", curl\_easy_strerror(res));**

### 5. Memory freeing:
  * Cleanup performed using: **curl_easy_cleanup(curl);**
  * Freed the custom header file list using **curl_slist_free_all(headers);**
