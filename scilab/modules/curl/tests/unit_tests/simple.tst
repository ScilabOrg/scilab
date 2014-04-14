curl = curl_easy_init();
curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
/* example.com is redirected, so we tell libcurl to follow redirection */
curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

res = curl_easy_perform(curl);
if(res <> CURLE_OK)
    mprintf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res)));
end

/* always cleanup */
curl_easy_cleanup(curl);

