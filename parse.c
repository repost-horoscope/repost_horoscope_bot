#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <curl/curl.h>

#define KARMA_DECAY_URL "http://karmadecay.com"
#define KARMA_DECAY_URL_LENGTH 22

#define USER_AGENT_STRING "bot to post horoscopes of reposts, by /u/palida"

//we dont want stdout! silly libcurl
struct data_and_size
{
  char *data;
  size_t size;
};

//function to call instead of dumping into stdout
static size_t write_to_memory(void *ptr, size_t size, size_t nmemb, void *userdata)
{
  //how many are there?
  size_t actual_size = size * nmemb;
  //i miss reinterpret_cast :(
  struct data_and_size *user_data = (struct data_and_size *)userdata;

  //allocate memory, need one extra for '\0'
  user_data->data= realloc(user_data->data, user_data->size + actual_size + 1);

  //oh noes!
  if(user_data->data == NULL)
  {
    fprintf(stderr, "ERROR: not enough memory to allocate for sexy new interwebs\n");
    //if we fail to realloc by 1 byte, we deserve the only error that can happen
    //return 0;
    //but i suppose not, silly best practices :/
    //so instead we gurantee it will throw an error because the size will not match
    return actual_size+1;
  }

  //copy the new data in, starting at '\0'
  memcpy(&(user_data->data[user_data->size]), ptr, actual_size);
  //update the size of the data in the struct
  user_data->size += actual_size;
  //null terminate!
  user_data->data[user_data->size] = '\0';

  //return amount of data allocated
  return actual_size;
}

//takes the given link and adds it to karmadecay
int parse_karmadecay(struct data_and_size *the_result, char *image_url)
{
  CURL *curl;

  //make sure we have enough room for karmadecay_url and image_urls
  char karmadecay_url[22+strlen(KARMA_DECAY_URL)];
  //copy karmadecay_url first
  strcpy(karmadecay_url, KARMA_DECAY_URL);
  //append with image_url
  strcat(karmadecay_url, image_url);

  //get easy handle
  curl = curl_easy_init();
  //for errors
  CURLcode result;

  //whats our url?
  printf("%s\n", karmadecay_url);

  if(curl)
  {
    //set up the url
    curl_easy_setopt(curl, CURLOPT_URL, karmadecay_url);
    //set up the user-agent
    curl_easy_setopt(curl, CURLOPT_USERAGENT, USER_AGENT_STRING);
    //set up struct to store data in
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)the_result);
    //set up the function to handle writing the data (not you silly stdout!)
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_memory);

    //send curl to interwebs
    result = curl_easy_perform(curl);

    //error(s)?
    if(result != CURLE_OK)
    {
      fprintf(stderr, "ERROR: curl_easy_perform() has failed: %s \n", curl_easy_strerror(result));

    curl_easy_cleanup(curl);

    return 1;
    }
  }

  return 0;
}

int main()
{
  //struct to hold all our lovely data
  struct data_and_size the_result;
  the_result.data = NULL;
  the_result.size = 0;

  //image url
  char image_url[1];
  image_url[0] = '\0';

  //need to call this to set up curl
  curl_global_init(CURL_GLOBAL_ALL);

  //lets try to parse karamdecay
  if(parse_karmadecay(&the_result, image_url) != 0)
  {
    fprintf(stderr, "ERROR: parsing karmadecay has failed :(\n");
    return 1;
  }

  //all done!
  curl_global_cleanup();

  return 0;
}
