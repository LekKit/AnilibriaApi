//
// Created by kotb on 07.08.2021.
//

#include "anilibria.hpp"

template <typename Container>
static std::string join_libr(const char sep,
                      const Container &vec) {
    if (vec.empty()) return "";

    std::string out;

    for (auto &value : vec) {
        out += value;
        out += sep;
    }

    out.pop_back();

    return out;
}

static size_t write_cb_libria(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string curl_build_query(CURL *curl,
                             const std::map<std::string, std::string> &query) {
    std::string qs;

    for (auto &entry : query) {
        char *qs_c = curl_easy_escape(curl, entry.second.c_str(),
                                      static_cast<int>(entry.second.size()));

        qs += entry.first;
        qs += '=';
        qs += std::string(qs_c);
        curl_free(qs_c);

        qs += '&';
    }

    qs.pop_back();

    return qs;
}

AniAPI::Title
AniAPI::Anilibria::get_title(uint32_t id, const AniAPI::strings_list_t &filter, const AniAPI::strings_list_t &remove) {

    std::map<std::string, std::string> q = {
            {"id", std::to_string(id)},
            {"filter", join_libr(',', filter)},
            {"remove", join_libr(',', remove)}
    };

    auto json = libria_method("v2", "getTitle", q);

    Title title = Title::from_json(json);

    return title;
}

std::vector<AniAPI::Title> AniAPI::Anilibria::get_titles(const std::string &id_list, const AniAPI::strings_list_t &filter,
                                                 const AniAPI::strings_list_t &remove) {
    auto json = libria_method("v2", "getTitle", {
        {"id_list", id_list},
        {"filter", join_libr(',', filter)},
        {"remove", join_libr(',', remove)}
    });

    std::vector<Title> titles{};

    for (auto &x : json.json.items()) {
        JSONProvider js(x.value());
        titles.push_back(Title::from_json(js));
    }

    return titles;
}

std::vector<AniAPI::Title> AniAPI::Anilibria::get_changes(uint32_t limit, uint32_t since,
                                                          uint32_t after, const strings_list_t &filter,
                                                          const strings_list_t &remove) {
    auto json = libria_method("v2", "getChanges", {
      {"limit", std::to_string(limit)},
      {"since", std::to_string(since)},
      {"after", std::to_string(after)},
      {"filter", join_libr(',', filter)},
      {"remove", join_libr(',', remove)}
    });

    std::vector<Title>


}

AniAPI::JSONProvider AniAPI::Anilibria::libria_method(const std::string &api_ver, const std::string &method,
                                                const std::map<std::string, std::string> &query) {
    std::string response;

    CURL* curl = curl_easy_init();
    std::string url = "http://api."+host_url+"/"+api_ver+"/"+method+"?";
    url += curl_build_query(curl, query);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb_libria);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        throw std::runtime_error("API ERROR");
    }


    curl_easy_cleanup(curl);
    return JSONProvider(response);
}
