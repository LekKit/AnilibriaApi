
#pragma once
#include <string>
#include <vector>
#include "anilibria_types.hpp"
#include <curl/curl.h>
#include <nlohmann/json.hpp>


namespace anilibria {
    // struct AdditionalFields;
    // struct TitleRequest;
    // struct ScheduleRequest;


    class AnilibriaAPI {

    private:
        std::string version = "v2";
        std::string base_url = "https://api.anilibria.tv";
        std::string auth_url = "https://wwnd.space";
        std::string session;

        void set_error_info();
        void set_error_info(nlohmann::json jsonarray);
        void set_error_info(int error_code, std::string error_msg);


    public:
        std::vector<Title> get_titles(std::vector<TitleRequest> request);
        std::vector<Title> get_updates(AdditionalFields request);
        std::vector<Title> get_changes(AdditionalFields request);
        std::vector<Schedule> get_schedule(ScheduleRequest request);
        std::vector<std::string> get_caching_nodes();
        std::vector<int> get_years();
        std::vector<std::string> get_genres(int sorting_type);
        std::vector<Title> search_titles(SearchTitlesRequest request);
        std::vector<Title> advanced_search(AdvancedSearchRequest);
        std::vector<TorrentUserStat> get_seed_stats(SeedStatsRequest request);
        std::vector<Title> get_favorites(AdditionalFields request);
        std::vector<Youtube> get_youtube(AdditionalFields request);
        std::vector<Feed> get_feed(AdditionalFields request);

        Title get_title(TitleRequest request);
        Title get_random_title(AdditionalFields request);

        Team get_team();

        std::string get_RSS(RSSRequest request);

        bool add_favorite(int title_id);
        bool del_favorite(int title_id);

        std::string auth(std::string mail, std::string passwd);
        bool logout();

        AnilibriaAPI(std::string base_url, std::string auth_url);
        AnilibriaAPI(std::string urls[2]);
        AnilibriaAPI(std::string session);
        AnilibriaAPI(std::string base_url, std::string auth_url, std::string session);

        void set_api_version(std::string version) {
          this->version = version;
        };

        bool error_occured = false;
        Error last_error;




    };



}
