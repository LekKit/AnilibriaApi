
#include <iostream>
#include <string>
#include <vector>
#include "anilibria_api.hpp"
#include "anilibria_types.hpp"
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <unordered_map>

static std::string url_builder(std::unordered_map<std::string, std::string> params){
  std::string url_addition = "";
  for (auto i : params) {
    if (i.second != "" && i.second != "-1") {
      url_addition.append("&" + i.first + "=" + i.second);
    }

  }
  if (url_addition.length() > 0) return "?" + url_addition.substr(1, -1);
  else return "";
}

static size_t write_callback(char* contents, size_t size, size_t nmemb, void* ptr) {

  ((std::string*)ptr)->append((char*)contents, size * nmemb);
  return size * nmemb;
};

static size_t header_callback(char* header, size_t size, size_t nmemb, void* ptr) {
  std::string strokk(header, size*nmemb);
    ((std::string*)ptr)->append(strokk);

    return size * nmemb;
};

/*
STATIC DECLORATION ENDS HERE
AnilibriaAPI CONSTRUCTORS DECLORATION STARTS HERE
*/



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

/*
AnilibriaAPI CONSTRUCTORS DECLORATION ENDS HERE
PRIVATE FUNCTIONS DECLORATION STARTS HERE
*/

void anilibria::AnilibriaAPI::set_error_info() {
  error_occured = false;
}

void anilibria::AnilibriaAPI::set_error_info(anilibria::Error error) {
  error_occured = true;
  last_error = error;

}

void anilibria::AnilibriaAPI::set_error_info(int error_code, std::string error_msg) {
  error_occured = true;
  last_error = anilibria::Error();
  last_error.code = error_code;
  last_error.message = error_msg;
}

/*
PRIVATE FUNCTIONS DECLORATION ENDS HERE
PULIC FUNCTIONS DECLORATION STARTS HERE
*/


std::string anilibria::AnilibriaAPI::auth(std::string mail, std::string passwd) {
  CURL* curl_handle = curl_easy_init();
  CURLcode res;

  std::string response_body;
  std::string response_header;

  curl_httppost* post = NULL;
  curl_httppost* last = NULL;

  curl_formadd(&post, &last, CURLFORM_COPYNAME, "mail", CURLFORM_COPYCONTENTS, mail.c_str(), CURLFORM_END);
  curl_formadd(&post, &last, CURLFORM_COPYNAME, "passwd", CURLFORM_COPYCONTENTS, passwd.c_str(), CURLFORM_END);

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

    if (jsonobj["err"]=="ok") {
      std::string token;
      token = response_header.substr(response_header.find("Set-Cookie: ")+22, -1);
      token = token.substr(0, token.find(";"));
      session = token;

      this->set_error_info();
      curl_easy_cleanup(curl_handle);
      return token;
    } else {
      anilibria::AnilibriaAPI::set_error_info(-1, jsonobj["mes"]);
      curl_easy_cleanup(curl_handle);

    }
  };
  return "";

};

bool anilibria::AnilibriaAPI::logout(){
  if (session == "") {
    return false;
  } else {
    session = "";
    return true;
  }
}

std::vector<anilibria::Title> anilibria::AnilibriaAPI::get_titles(std::vector<anilibria::TitleRequest> request) {
  std::string url = base_url + "/" + version + "/" + "getTitles";
  std::string id_list;
  std::string code_list;
  std::vector<anilibria::Title> titles;
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
    CURLcode res;
    if (curl_handle) {
      std::string response_body;
      curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
      curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

      res = curl_easy_perform(curl_handle);



      if (!res) {
        nlohmann::json jsonarray = nlohmann::json::parse(response_body);
        if (!jsonarray.contains("error")) {
          for (auto it : jsonarray) titles.push_back(anilibria::Title(it));
          this->set_error_info();
        } else {
          this->set_error_info(anilibria::Error(jsonarray));
        }
      } else {
        this->set_error_info(res, "CURL_ERROR");
      }
      curl_easy_cleanup(curl_handle);
      return titles;
    }
    this->set_error_info(0, "CURL_ERROR");
    curl_easy_cleanup(curl_handle);
    return titles;

  }

}

std::vector<anilibria::Title> anilibria::AnilibriaAPI::get_updates(anilibria::AdditionalFields request) {
  std::string url = base_url + "/" + version + "/" + "getUpdates";
  std::vector<anilibria::Title> titles;

  std::unordered_map<std::string, std::string> params;
  params["filter"] = request.filter;
  params["include"] = request.include;
  params["remove"] = request.remove;
  params["description_type"] = request.description_type;
  params["playlist_type"] = request.playlist_type;
  params["limit"] = std::to_string(request.limit);
  params["since"] = std::to_string(request.since);
  params["after"] = std::to_string(request.after);

  url += url_builder(params);
  CURL* curl_handle = curl_easy_init();
  CURLcode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform(curl_handle);



    if (!res) {

      nlohmann::json jsonarray = nlohmann::json::parse(response_body);

      if (!jsonarray.contains("error")) {
        for (auto it : jsonarray) titles.push_back(anilibria::Title(it));
        this->set_error_info();
      } else {
        this->set_error_info(anilibria::Error(jsonarray));
      }
    } else {
      this->set_error_info(res, "CURL_ERROR");
    }
    curl_easy_cleanup(curl_handle);
    return titles;
  }
  this->set_error_info(0, "CURL_ERROR");
  curl_easy_cleanup(curl_handle);
  return titles;
}

std::vector<anilibria::Title> anilibria::AnilibriaAPI::get_changes(anilibria::AdditionalFields request) {
  std::string url = base_url + "/" + version + "/" + "getChanges";
  std::vector<anilibria::Title> titles;

  std::unordered_map<std::string, std::string> params;
  params["filter"] = request.filter;
  params["include"] = request.include;
  params["remove"] = request.remove;
  params["description_type"] = request.description_type;
  params["limit"] = std::to_string(request.limit);
  params["since"] = std::to_string(request.since);
  params["after"] = std::to_string(request.after);

  url += url_builder(params);
  CURL* curl_handle = curl_easy_init();
  CURLcode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform(curl_handle);



    if (!res) {

      nlohmann::json jsonarray = nlohmann::json::parse(response_body);

      if (!jsonarray.contains("error")) {
        for (auto it : jsonarray) titles.push_back(anilibria::Title(it));
        this->set_error_info();
      } else {
        this->set_error_info(anilibria::Error(jsonarray));
      }
    } else {
      this->set_error_info(res, "CURL_ERROR");
    }
    curl_easy_cleanup(curl_handle);
    return titles;
  }
  this->set_error_info(0, "CURL_ERROR");
  curl_easy_cleanup(curl_handle);
  return titles;
}

std::vector<anilibria::Schedule> anilibria::AnilibriaAPI::get_schedule(anilibria::ScheduleRequest request) {
  std::string url = base_url + "/" + version + "/" + "getSchedule";
  std::vector<anilibria::Schedule> schedule;

  std::unordered_map<std::string, std::string> params;
  params["filter"] = request.filter;
  params["include"] = request.include;
  params["remove"] = request.remove;
  params["description_type"] = request.description_type;
  params["playlist_type"] = request.playlist_type;
  params["days"] = request.days;

  url += url_builder(params);
  CURL* curl_handle = curl_easy_init();
  CURLcode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform(curl_handle);


    if (!res) {

      nlohmann::json jsonarray = nlohmann::json::parse(response_body);
      
      if (!jsonarray.contains("error")) {
        for (auto it : jsonarray) schedule.push_back(anilibria::Schedule(it));
        this->set_error_info();
      } else {
        this->set_error_info(anilibria::Error(jsonarray));
      }
    } else {
      this->set_error_info(res, "CURL_ERROR");
    }
    curl_easy_cleanup(curl_handle);
    return schedule;
  }
  this->set_error_info(0, "CURL_ERROR");
  curl_easy_cleanup(curl_handle);
  return schedule;
}

std::vector<std::string> anilibria::AnilibriaAPI::get_caching_nodes() {
  std::string url = base_url + "/" + version + "/" + "getCachingNodes";

  CURL* curl_handle = curl_easy_init();
  CURLcode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform(curl_handle);

    nlohmann::json jsonarray = nlohmann::json::parse(response_body);

    if (!res) {

      nlohmann::json jsonarray = nlohmann::json::parse(response_body);

      if (!jsonarray.contains("error")) {
        this->set_error_info();
        curl_easy_cleanup(curl_handle);
        return jsonarray.get<std::vector<std::string>>();
      } else {
        this->set_error_info(anilibria::Error(jsonarray));
      }
    } else {
      this->set_error_info(res, "CURL_ERROR");
    }
    curl_easy_cleanup(curl_handle);
    return std::vector<std::string> ();
  }
  this->set_error_info(0, "CURL_ERROR");
  curl_easy_cleanup(curl_handle);
  return std::vector<std::string> ();
}

std::vector<int> anilibria::AnilibriaAPI::get_years() {
  std::string url = base_url + "/" + version + "/" + "getYears";

  CURL* curl_handle = curl_easy_init();
  CURLcode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);
    res = curl_easy_perform(curl_handle);



    if (!res) {

      nlohmann::json jsonarray = nlohmann::json::parse(response_body);

      if (!jsonarray.contains("error")) {
        this->set_error_info();
        curl_easy_cleanup(curl_handle);
        return jsonarray.get<std::vector<int>>();
      } else {
        this->set_error_info(anilibria::Error(jsonarray));
      }
    } else {
      this->set_error_info(res, "CURL_ERROR");
    }
    curl_easy_cleanup(curl_handle);
    return std::vector<int> ();
  }
  this->set_error_info(0, "CURL_ERROR");
  curl_easy_cleanup(curl_handle);
  return std::vector<int> ();
}

std::vector<std::string> anilibria::AnilibriaAPI::get_genres(int sorting_type) {
  std::string url = base_url + "/" + version + "/" + "getGenres";

  std::unordered_map<std::string, std::string> params;
  params["sorting_type"] = std::to_string(sorting_type);

  url += url_builder(params);
  CURL* curl_handle = curl_easy_init();
  CURLcode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform(curl_handle);


    if (!res) {

      nlohmann::json jsonarray = nlohmann::json::parse(response_body);

      if (!jsonarray.contains("error")) {
        this->set_error_info();
        curl_easy_cleanup(curl_handle);
        return jsonarray.get<std::vector<std::string>>();
      } else {
        this->set_error_info(anilibria::Error(jsonarray));
      }
    } else {
      this->set_error_info(res, "CURL_ERROR");
    }
    curl_easy_cleanup(curl_handle);
    return std::vector<std::string> ();
  }
  this->set_error_info(0, "CURL_ERROR");
  curl_easy_cleanup(curl_handle);
  return std::vector<std::string> ();
}

std::vector<anilibria::Title> anilibria::AnilibriaAPI::search_titles(anilibria::SearchTitlesRequest request) {
  std::string url = base_url + "/" + version + "/" + "searchTitles";
  std::vector<anilibria::Title> titles;

  std::unordered_map<std::string, std::string> params;
  params["search"] = request.search;
  params["year"] = request.year;
  params["season_code"] = request.season_code;
  params["genres"] = request.genres;
  params["voice"] = request.voice;
  params["translator"] = request.translator;
  params["editing"] = request.editing;
  params["decor"] = request.decor;
  params["timing"] = request.timing;

  params["filter"] = request.filter;
  params["remove"] = request.remove;
  params["include"] = request.include;
  params["description_type"] = request.description_type;
  params["playlist_type"] = request.playlist_type;
  params["limit"] = std::to_string(request.limit);
  params["after"] = std::to_string(request.after);

  url += url_builder(params);
  CURL* curl_handle = curl_easy_init();
  CURLcode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform(curl_handle);

    if (!res) {

      nlohmann::json jsonarray = nlohmann::json::parse(response_body);

        if (!jsonarray.contains("error")) {
          for (auto it : jsonarray) titles.push_back(anilibria::Title(it));
          this->set_error_info();
        } else {
          this->set_error_info(anilibria::Error(jsonarray));
        }
      } else {
        this->set_error_info(res, "CURL_ERROR");
      }
      curl_easy_cleanup(curl_handle);
      return titles;
  }
  this->set_error_info(0, "CURL_ERROR");
  curl_easy_cleanup(curl_handle);
  return titles;
}

std::vector<anilibria::Title> anilibria::AnilibriaAPI::advanced_search(anilibria::AdvancedSearchRequest request) {
  std::string url = base_url + "/" + version + "/" + "advancedSearch";
  std::vector<anilibria::Title> titles;

  std::unordered_map<std::string, std::string> params;
  params["query"] = request.query;

  params["filter"] = request.filter;
  params["remove"] = request.remove;
  params["include"] = request.include;
  params["description_type"] = request.description_type;
  params["playlist_type"] = request.playlist_type;
  params["limit"] = std::to_string(request.limit);
  params["after"] = std::to_string(request.after);

  params["order_by"] = request.order_by;
  params["sort_direction"] = std::to_string(request.sort_direction);

  url += url_builder(params);
  CURL* curl_handle = curl_easy_init();
  CURLcode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform(curl_handle);

    if (!res) {

      nlohmann::json jsonarray = nlohmann::json::parse(response_body);

        if (!jsonarray.contains("error")) {
          for (auto it : jsonarray) titles.push_back(anilibria::Title(it));
          this->set_error_info();
        } else {
          this->set_error_info(anilibria::Error(jsonarray));
        }
      } else {
        this->set_error_info(res, "CURL_ERROR");
      }
      curl_easy_cleanup(curl_handle);
      return titles;
  }
  this->set_error_info(0, "CURL_ERROR");
  curl_easy_cleanup(curl_handle);
  return titles;
}

std::vector<anilibria::TorrentUserStat> anilibria::AnilibriaAPI::get_seed_stats(anilibria::SeedStatsRequest request) {
  std::string url = base_url + "/" + version + "/" + "getSeedStats";
  std::vector<anilibria::TorrentUserStat> users;

  std::unordered_map<std::string, std::string> params;
  params["users"] = request.users;

  params["filter"] = request.filter;
  params["remove"] = request.remove;
  params["include"] = request.include;
  params["description_type"] = request.description_type;
  params["playlist_type"] = request.playlist_type;
  params["limit"] = std::to_string(request.limit);
  params["after"] = std::to_string(request.after);

  params["sort_by"] = request.sort_by;
  params["order"] = std::to_string(request.order);

  url += url_builder(params);
  CURL* curl_handle = curl_easy_init();
  CURLcode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform(curl_handle);

    if (!res) {

      nlohmann::json jsonarray = nlohmann::json::parse(response_body);

        if (!jsonarray.contains("error")) {
          for (auto it : jsonarray) users.push_back(anilibria::TorrentUserStat(it));
          this->set_error_info();
        } else {
          this->set_error_info(anilibria::Error(jsonarray));
        }
      } else {
        this->set_error_info(res, "CURL_ERROR");
      }
      curl_easy_cleanup(curl_handle);
      return users;
  }
  this->set_error_info(0, "CURL_ERROR");
  curl_easy_cleanup(curl_handle);
  return users;
}

std::vector<anilibria::Title> anilibria::AnilibriaAPI::get_favorites(anilibria::AdditionalFields request) {
  std::string url = base_url + "/" + version + "/" + "getFavorites";
  std::vector<anilibria::Title> titles;

  std::unordered_map<std::string, std::string> params;
  params["session"] = session;

  params["filter"] = request.filter;
  params["remove"] = request.remove;
  params["include"] = request.include;
  params["description_type"] = request.description_type;
  params["playlist_type"] = request.playlist_type;

  url += url_builder(params);
  CURL* curl_handle = curl_easy_init();
  CURLcode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform(curl_handle);  nlohmann::json jsonarray = nlohmann::json::parse(response_body);

    if (!res) {

      nlohmann::json jsonarray = nlohmann::json::parse(response_body);

        if (!jsonarray.contains("error")) {
          for (auto it : jsonarray) titles.push_back(anilibria::Title(it));
          this->set_error_info();
        } else {
          this->set_error_info(anilibria::Error(jsonarray));
        }
      } else {
        this->set_error_info(res, "CURL_ERROR");
      }
      curl_easy_cleanup(curl_handle);
      return titles;
  }
  this->set_error_info(0, "CURL_ERROR");
  curl_easy_cleanup(curl_handle);
  return titles;
}

std::vector<anilibria::Youtube> anilibria::AnilibriaAPI::get_youtube(anilibria::AdditionalFields request) {
  std::string url = base_url + "/" + version + "/" + "getYoutube";
  std::vector<anilibria::Youtube> youtube;

  std::unordered_map<std::string, std::string> params;
  params["filter"] = request.filter;
  params["remove"] = request.remove;
  params["limit"] = std::to_string(request.limit);
  params["since"] = std::to_string(request.since);
  params["after"] = std::to_string(request.after);

  url += url_builder(params);
  CURL* curl_handle = curl_easy_init();
  CURLcode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform(curl_handle);

    if (!res) {

      nlohmann::json jsonarray = nlohmann::json::parse(response_body);

        if (!jsonarray.contains("error")) {
          for (auto it : jsonarray) youtube.push_back(anilibria::Youtube(it));
          this->set_error_info();
        } else {
          this->set_error_info(anilibria::Error(jsonarray));
        }
      } else {
        this->set_error_info(res, "CURL_ERROR");
      }
      curl_easy_cleanup(curl_handle);
      return youtube;
  }
  this->set_error_info(0, "CURL_ERROR");
  curl_easy_cleanup(curl_handle);
  return youtube;
}

std::vector<anilibria::Feed> anilibria::AnilibriaAPI::get_feed(anilibria::AdditionalFields request) {
  std::string url = base_url + "/" + version + "/" + "getFeed";
  std::vector<anilibria::Feed> feed;

  std::unordered_map<std::string, std::string> params;
  params["filter"] = request.filter;
  params["remove"] = request.remove;
  params["include"] = request.include;
  params["limit"] = std::to_string(request.limit);
  params["since"] = std::to_string(request.since);
  params["after"] = std::to_string(request.after);
  params["description_type"] = request.description_type;
  params["playlist_type"] = request.playlist_type;

  url += url_builder(params);

  CURL* curl_handle = curl_easy_init();
  CURLcode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform(curl_handle);

    if (!res) {
      nlohmann::json jsonarray = nlohmann::json::parse(response_body);
        if (!jsonarray.contains("error")) {
          for (auto it : jsonarray) feed.push_back(anilibria::Feed(it));
          this->set_error_info();
        } else {
          this->set_error_info(anilibria::Error(jsonarray));
        }
      } else {
        this->set_error_info(res, "CURL_ERROR");
      }
      curl_easy_cleanup(curl_handle);
      return feed;
  }
  this->set_error_info(0, "CURL_ERROR");
  curl_easy_cleanup(curl_handle);
  return feed;
}

anilibria::Title anilibria::AnilibriaAPI::get_title(anilibria::TitleRequest request) {
  std::string url = base_url + "/" + version + "/" + "getTitle";

  std::unordered_map<std::string, std::string> params;
  params["id"] = std::to_string(request.id);
  params["code"] = request.code;
  params["torrent_id"] = std::to_string(request.torrent_id);

  params["filter"] = request.filter;
  params["remove"] = request.remove;
  params["include"] = request.include;
  params["description_type"] = request.description_type;
  params["playlist_type"] = request.playlist_type;

  url += url_builder(params);
  CURL* curl_handle = curl_easy_init();
  CURLcode res;
  if (curl_handle && request.id != -1 && request.code != "") {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform(curl_handle);nlohmann::json jsonarray = nlohmann::json::parse(response_body);

    if (!res) {

      nlohmann::json jsonarray = nlohmann::json::parse(response_body);

      if (!jsonarray.contains("error")) {
        this->set_error_info();
        curl_easy_cleanup(curl_handle);
        return anilibria::Title(jsonarray);
      } else {
        this->set_error_info(anilibria::Error(jsonarray));
      }
    } else {
      this->set_error_info(res, "CURL_ERROR");
    }
  }
  this->set_error_info(0, "CURL_ERROR");
  curl_easy_cleanup(curl_handle);
  return anilibria::Title();
}

anilibria::Title anilibria::AnilibriaAPI::get_random_title(anilibria::AdditionalFields request) {
  std::string url = base_url + "/" + version + "/" + "getRandomTitle";

  std::unordered_map<std::string, std::string> params;
  params["filter"] = request.filter;
  params["remove"] = request.remove;
  params["include"] = request.include;
  params["description_type"] = request.description_type;
  params["playlist_type"] = request.playlist_type;

  url += url_builder(params);
  CURL* curl_handle = curl_easy_init();
  CURLcode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform(curl_handle);

    if (!res) {

      nlohmann::json jsonarray = nlohmann::json::parse(response_body);

      if (!jsonarray.contains("error")) {
        this->set_error_info();
        curl_easy_cleanup(curl_handle);
        return anilibria::Title(jsonarray);
      } else {
        this->set_error_info(anilibria::Error(jsonarray));
      }
    } else {
      this->set_error_info(res, "CURL_ERROR");
    }
  }
  this->set_error_info(0, "CURL_ERROR");
  curl_easy_cleanup(curl_handle);
  return anilibria::Title();
}

anilibria::Team anilibria::AnilibriaAPI::get_team() {
  std::string url = base_url + "/" + version + "/" + "getTeam";

  CURL* curl_handle = curl_easy_init();
  CURLcode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform(curl_handle);

    if (!res) {

      nlohmann::json jsonarray = nlohmann::json::parse(response_body);

      if (!jsonarray.contains("error")) {
        this->set_error_info();
        curl_easy_cleanup(curl_handle);
        return anilibria::Team(jsonarray);
      } else {
        this->set_error_info(anilibria::Error(jsonarray));
      }
    } else {
        this->set_error_info(res, "CURL_ERROR");
    }
  }
    curl_easy_cleanup(curl_handle);
    return anilibria::Team();
  }

std::string anilibria::AnilibriaAPI::get_RSS(anilibria::RSSRequest request) {
  std::string url = base_url + "/" + version + "/" + "getRSS";

  std::unordered_map<std::string, std::string> params;
  params["rss_type"] = request.rss_type;
  params["session"] = session;

  params["limit"] = std::to_string(request.limit);
  params["since"] = std::to_string(request.since);
  params["after"] = std::to_string(request.after);

  url += url_builder(params);
  CURL* curl_handle = curl_easy_init();
  CURLcode res;
  if (curl_handle) {
    std::string response_body;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

    res = curl_easy_perform(curl_handle);
    nlohmann::json jsonarray = nlohmann::json();


    if (!res) {
      try {
        nlohmann::json jsonarray = nlohmann::json::parse(response_body);
      } catch(nlohmann::json::parse_error& e) {
        nlohmann::json jsonarray = nlohmann::json();
      }
      if (!jsonarray.contains("error")) {
        this->set_error_info();
        curl_easy_cleanup(curl_handle);
        return response_body;
      } else {
        this->set_error_info(anilibria::Error(jsonarray));
      }
    } else {
      this->set_error_info(res, "CURL_ERROR");
    }
  }
  this->set_error_info(0, "CURL_ERROR");
  curl_easy_cleanup(curl_handle);
  return "";
  }

bool anilibria::AnilibriaAPI::add_favorite(int title_id) {
  if (session == "") {
    this->set_error_info(-3, "Session not specified, please do authorization first");
    return false;
  } else {
    std::string url = base_url + "/" + version + "/" + "addFavorite";

    std::unordered_map<std::string, std::string> params;
    params["session"] = session;
    params["title_id"] = std::to_string(title_id);

    url += url_builder(params);
    CURL* curl_handle = curl_easy_init();
    CURLcode res;
    if (curl_handle) {
      std::string response_body;
      curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl_handle, CURLOPT_PUT, 1);
      curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
      curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

      res = curl_easy_perform(curl_handle);

      if (!res) {

        nlohmann::json jsonarray = nlohmann::json::parse(response_body);

        if (!jsonarray.contains("error")) {
          this->set_error_info();
          curl_easy_cleanup(curl_handle);
          return true;
        } else {
          this->set_error_info(anilibria::Error(jsonarray));
        }
      } else {
        this->set_error_info(res, "CURL_ERROR");
      }
    }
      curl_easy_cleanup(curl_handle);
      return false;

    }
  }

bool anilibria::AnilibriaAPI::del_favorite(int title_id) {
  if (session == "") {
    this->set_error_info(-3, "Session not specified, please do authorization first");
    return false;
  } else {
    std::string url = base_url + "/" + version + "/" + "delFavorite";

    std::unordered_map<std::string, std::string> params;
    params["session"] = session;
    params["title_id"] = std::to_string(title_id);

    url += url_builder(params);
    CURL* curl_handle = curl_easy_init();
    CURLcode res;
    if (curl_handle) {
      std::string response_body;
      curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, "DELETE");
      curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
      curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_body);

      res = curl_easy_perform(curl_handle);

      if (!res) {

        nlohmann::json jsonarray = nlohmann::json::parse(response_body);

        if (!jsonarray.contains("error")) {
          this->set_error_info();
          curl_easy_cleanup(curl_handle);
          return true;
        } else {
          this->set_error_info(anilibria::Error(jsonarray));
        }
      } else {
        this->set_error_info(res, "CURL_ERROR");
      }
    }
      curl_easy_cleanup(curl_handle);
      return false;
    }
  }
