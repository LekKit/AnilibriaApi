#include <iostream>
#include <string>
#include <vector>
#include "anilibria_api.hpp"
#include "anilibria_types.hpp"
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <unordered_map>

static std::string url_builder(std::unordered_map<std::string, std::string> params){
  std::string url_addition;
  for (auto i : params) {
    if (i.second != "") {
      url.append("&" + i.first + "=" + std::to_string(i.second));
  }
  return url_addition.substr(1, -1);
}

static size_t write_callback(char* contents, size_t size, size_t nmemb, void* ptr) {

  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
};

static size_t header_callback(char* header, size_t size, size_t nmemb, void* ptr) {
  std::string strokk(contents, size*nmemb);
    ((std::string*)userp)->append(strokk);

    return size * nmemb;
};


anilibria::AnilibriaAPI::AnilibriaAPI(std::string session) {
  this->session = session;

};

anilibria::AnilibriaAPI::AnilibriaAPI(std::string urls[2]) {
  base_url = urls[0];
  auth_url = urls[1];
};

anilibria::AnilibriaAPI::AnilibriaAPI(std::string base_url, std::string auth_url) {
  this->base_url = base_url;
  this->auth_url = auth_url;
};

anilibria::AnilibriaAPI::AnilibriaAPI(std::string base_url, std::string auth_url, std::string session) {
  this->base_url = base_url;
  this->auth_url = auth_url;
  this->session = session;
};


std::string anilibria::AnilibriaAPI::auth(std::string mail, std::string passwd) {
  CURL* curl_handle = curl_easy_init();
  CURLCode res;

  std::string response_body;
  std::string response_header;

  curl_httppost* post = NULL;
  curl_httppost* last = NULL;

  curl_formadd(&post, &last, CURLFORM_COPYNAME, "mail", CURLFROM_COPYCONTENTS, mail.c_str(), CURLFORM_END);
  curl_formadd(&post, &last, CURLFORM_COPYNAME, "passwd", CURLFROM_COPYCONTENTS, passwd.c_str(), CURLFORM_END);

  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, header_callback);
  curl_easy_setopt(curl_handle, CURLOPT_URL, auth_url+"/public/login.php");
  curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
  curl_easy_setopt(curl_handle, CURLOPT_HTTPPOST, post);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_header);

  res = curl_easy_perform(curl_handle);



  if (!res)
  {
    nlohmann::json jsonobj = nlohmann::json::parse(response_body);

    if (response_body["err"]=="ok") {
      std::string token;
      token = response_header.substr(response_header.find("Set-Cookie: ")+22, -1);
      token = token.substr(0, token.find(";"));
      session = token;

      curl_easy_cleanup(curl_handle);
      return token;
    }
  };
  return "";

};

bool anilibria::AnilibriaAPI::logout(){
  if (!session) {
    return false;
  } else {
    session = "";
    return true;
  }
}

std::vector<anilibria::Title> get_titles(std::vector<anilibria::TitleRequest> request) {
  std::string url = base_url + "/" + version + "/" + "getTitles?";
  std::string response_body;
  std::string id_list;
  std::string code_list;
  std::vector<anilibria::Title> titles();
  for (anilibria::TitleRequest tr : request) {
    if (tr.id > -1) {
      id_list.append(std::to_string(tr.id) + ",");
    } else if (tr.code != "") {
      code_list.append(tr.code+",");
    }
  }
  if (code_list == "" && id_list == "") {
    return titles;
  } else {
    std::unordered_map<std::string, std::string> params;
    params["id_list"] = id_list;
    params["code_list"] = code_list;
    params["filter"] = request[0].filter;
    params["include"] = request[0].include;
    params["remove"] = request[0].remove;
    params["description_type"] = request[0].description_type;
    params["playlist_type"] = request[0].playlist_type;

    url += url_builder(params);
    CURL* curl_handle = curl_easy_init();
    CURLCode res;
    if (curl_handle) {
      std::string response_body;
      curl_easy_setopt(curl_handle, CURLOPT_URL, url);
      curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
      curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

      res = curl_easy_perform();

      nlohmann::json jsonarray = nlohmann::json::parse(response_body);

      if (!res) {
        if (!jsonarray.contains("error")) {
          for (auto it : jsonarray) titles.push_back(anilibria::Title(it, this));
        }
      }
      return titles;
    }
  }

}

std::vector<anilibria::Title> get_updates(anilibria::AdditionalFields request) {
  std::string url = base_url + "/" + version + "/" + "getUpdates?";
  std::vector<anilibria::Title> titles;
  std::string response_body;

  std::unordered_map<std::string, std::string> params;
  params["filter"] = request.filter;
  params["include"] = request.include;
  params["remove"] = request.remove;
  params["description_type"] = request.description_type;
  params["playlist_type"] = request.playlist_type;
  params["limit"] = request.limit;
  params["since"] = request.since;
  params["after"] = request.after;

  url += url_builder(params);
  CURL* curl_handle = curl_easy_init();
  CURLCode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform();

    nlohmann::json jsonarray = nlohmann::json::parse(response_body);

    if (!res) {
      if (!jsonarray.contains("error")) {
        for (auto it : jsonarray) titles.push_back(anilibria::Title(it, this));
      }
    }
    return titles;
  }
}

std::vector<anilibria::Title> get_changes(anilibria::AdditionalFields request) {
  std::string url = base_url + "/" + version + "/" + "getChanges?";
  std::vector<anilibria::Title> titles;
  std::string response_body;

  std::unordered_map<std::string, std::string> params;
  params["filter"] = request.filter;
  params["include"] = request.include;
  params["remove"] = request.remove;
  params["description_type"] = request.description_type;
  params["limit"] = request.limit;
  params["since"] = request.since;
  params["after"] = request.after;

  url += url_builder(params);
  CURL* curl_handle = curl_easy_init();
  CURLCode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform();

    nlohmann::json jsonarray = nlohmann::json::parse(response_body);

    if (!res) {
      if (!jsonarray.contains("error")) {
        for (auto it : jsonarray) titles.push_back(anilibria::Title(it, this));
      }
    }
    return titles;
  }
}

std::vector<anilibria::Schedule> get_schedule(anilibria::ScheduleRequest request) {
  std::string url = base_url + "/" + version + "/" + "getSchedule?";
  std::vector<anilibria::Schedule> schedule;
  std::string response_body;

  std::unordered_map<std::string, std::string> params;
  params["filter"] = request.filter;
  params["include"] = request.include;
  params["remove"] = request.remove;
  params["description_type"] = request.description_type;
  params["playlist_type"] = request.playlist_type;
  params["days"] = request.days;

  url += url_builder(params);
  CURL* curl_handle = curl_easy_init();
  CURLCode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform();

    nlohmann::json jsonarray = nlohmann::json::parse(response_body);

    if (!res) {
      if (!jsonarray.contains("error")) {
        for (auto it : jsonarray) titles.push_back(anilibria::Schedule(it, this));
      }
    }
    return schedule;
  }
}

std::vector<std::string> get_caching_nodes() {
  std::string url = base_url + "/" + version + "/" + "getCachingNodes";
  std::string response_body;

  CURL* curl_handle = curl_easy_init();
  CURLCode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform();

    nlohmann::json jsonarray = nlohmann::json::parse(response_body);

    if (!res) {
      if (!jsonarray.contains("error")) {
        return jsonarray;
      }
    }

}

std::vector<int> get_years() {
  std::string url = base_url + "/" + version + "/" + "getYears";
  std::string response_body;

  CURL* curl_handle = curl_easy_init();
  CURLCode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform();

    nlohmann::json jsonarray = nlohmann::json::parse(response_body);

    if (!res) {
      if (!jsonarray.contains("error")) {
        return jsonarray;
      }
    }

}

std::vector<std::string> get_genres(int sorting_type) {
  std::string url = base_url + "/" + version + "/" + "getGenres?";
  std::string response_body;

  std::unordered_map<std::string, std::string> params;
  params["sorting_type"] = sorting_type;

  url += url_builder(params);
  CURL* curl_handle = curl_easy_init();
  CURLCode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform();

    nlohmann::json jsonarray = nlohmann::json::parse(response_body);

    if (!res) {
      if (!jsonarray.contains("error")) {
        return jsonarray;
      }
    }
  }

}
