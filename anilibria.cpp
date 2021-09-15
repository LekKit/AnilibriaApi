//
// Created by kotb on 07.08.2021.
//

#include "anilibria.hpp"

typedef std::vector<std::string> strings_list_t;

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

    std::vector<Title> titles{};

    for (auto &x : json.json.items()) {
        JSONProvider js(x.value());
        titles.push_back(Title::from_json(js));
    }

    return titles;


}

std::vector<AniAPI::Schedule> AniAPI::Anilibria::get_schedule(const std::string &days, const strings_list_t &filter,
                                                              const strings_list_t &remove) {
    auto json = libria_method("v2", "getSchedule", {
      {"days", days},
      {"filter", join_libr(',', filter)},
      {"remove", join_libr(',', remove)}
    });

    std::vector<Schedule> schedule{};

    for (auto &x : json.json.items()) {
        JSONProvider js(x.value());
        schedule.push_back(Schedule::from_json(js));
    }

    return schedule;
}

std::vector<std::string> AniAPI::Anilibria::get_caching_nodes(){
    auto json = libria_method("v2", "getCachingNodes");

    std::vector<std::string> nodes{};

    for (auto &x : json.json.items()){
        nodes.push_back(x.value());
    }

    return nodes;
}

AniAPI::Title AniAPI::Anilibria::get_random_title(const strings_list_t &filter, const strings_list_t &remove) {
    auto json = libria_method("v2", "getRandomTitle", {
      {"filter", join_libr(',', filter)},
      {"remove", join_libr(',', remove)}
    });

    Title title = Title::from_json(json);

    return title;
}

std::vector<std::uint32_t> AniAPI::Anilibria::get_years() {
    auto json = libria_method("v2", "getYears");

    std::vector<uint32_t> years{};

    for (auto &x : json.json.items()){
        years.push_back(x.value());
    }

    return years;
}

std::vector<std::string> AniAPI::Anilibria::get_genres(uint8_t sorting_type) {
    auto json = libria_method("v2", "getGenres", {
      {"sorting_type", std::to_string(sorting_type)}
    });

    std::vector<std::string> genres{};

    for (auto &x : json.json.items()){
        genres.push_back(x.value());
    }

    return genres;
}

std::vector<AniAPI::Title> AniAPI::Anilibria::search_titles(const std::string &search, uint32_t limit,
                                         uint32_t after, const strings_list_t &year,
                                         const strings_list_t &season_code, const strings_list_t &genres,
                                         const strings_list_t &voice, const strings_list_t &translator,
                                         const strings_list_t &editing, const strings_list_t &decor,
                                         const strings_list_t &timing, const strings_list_t &filter,
                                         const strings_list_t &remove) {
    auto json = libria_method("v2", "searchTitles", {
      {"limit", std::to_string(limit)},
      {"after", std::to_string(after)},
      {"search", search},
      {"year", join_libr(',', year)},
      {"season_code", join_libr(',', season_code)},
      {"genres", join_libr(',', genres)},
      {"voice", join_libr(',', voice)},
      {"translator", join_libr(',', translator)},
      {"editing", join_libr(',', editing)},
      {"decor", join_libr(',', decor)},
      {"timing", join_libr(',', timing)},
      {"filter", join_libr(',', filter)},
      {"remove", join_libr(',', remove)}
    });

    std::vector<AniAPI::Title> titles{};

    for (auto &x : json.json.items()) {
        JSONProvider js(x.value());
        titles.push_back(Title::from_json(js));
    }

    return titles;
}

std::vector<AniAPI::Title> AniAPI::Anilibria::advanced_search(const std::string &query, uint32_t limit,
                                   uint32_t after, const std::string &order_by,
                                   uint8_t sort_direction, const strings_list_t &filter,
                                   const strings_list_t &remove) {

    auto json = libria_method("v2", "advancedSearch", {
      {"query", query},
      {"limit", std::to_string(limit)},
      {"after", std::to_string(after)},
      {"order_by", order_by},
      {"sort_direction", std::to_string(sort_direction)},
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

AniAPI::TranslationTeam AniAPI::Anilibria::get_team() {
    auto json = libria_method("v2", "getTeam");

    TranslationTeam team = TranslationTeam::from_json(json);

    return team;
}


AniAPI::JSONProvider AniAPI::Anilibria::libria_method(const std::string &api_ver, const std::string &method,
                                                const std::map<std::string, std::string> &query) {
    std::string response;

    #ifdef EBIL
    std::cout << "[DEBUG] Performing GET (http://api." << host_url << "/" << api_ver << "/" << method << ") request\n";
    #endif

    CURL* curl = curl_easy_init();
    std::string url = "http://api."+host_url+"/"+api_ver+"/"+method+"?";
    url += curl_build_query(curl, query);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb_libria);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // #ifdef EBIL
    // std::cout << "[DEBUG] Performing GET (" << url << ") request\n";
    // #endif

    CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        throw std::runtime_error("API ERROR");
    }


    curl_easy_cleanup(curl);
    return JSONProvider(response);
}

AniAPI::JSONProvider AniAPI::Anilibria::libria_method(const std::string &api_ver, const std::string &method) {
    std::string response;

    #ifdef EBIL
    std::cout << "[DEBUG] Performing GET (http://api." << host_url << "/" << api_ver << "/" << method << ") request\n";
    #endif

    CURL* curl = curl_easy_init();
    std::string url = "http://api."+host_url+"/"+api_ver+"/"+method;
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
