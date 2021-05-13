
#pragma once
#include <string>
#include <vector>
#include "anilibria_api.hpp"
#include <nlohmann/json.hpp>


namespace anilibria {
    class AnilibriaAPI;

    struct AdditionalFields {

        std::string filter;
        std::string remove;
        std::string include;
        std::string description_type;
        std::string playlist_type;
        int limit;
        int since;
        int after;



    };

    struct TitleRequest : public AdditionalFields {

        std::string code;
        int id = -1;
        int torrent_id = -1;

        TitleRequest(int id) {
            this->id = id;
            torrent_id = -1;
        };

        TitleRequest(std::string code) {
            this->code = code;
            id = -1;
            torrent_id = -1;
        };
      };

    struct ScheduleRequest : public AdditionalFields {

        std::string days;

    };

    struct SearchTitlesRequest : public AdditionalFields {

        std::string search;
        std::string year;
        std::string season_code;
        std::string genres;
        std::string voice;
        std::string translator;
        std::string editing;
        std::string decor;
        std::string timing;


    };

    struct AdvancedSearchRequest : public AdditionalFields {

        std::string query;
        std::string order_by;
        int sort_direction;

    };

    struct SeedStatsRequest : public AdditionalFields {

        std::string users;
        std::string sort_by;
        int order;
    };

    struct RSSRequest : public AdditionalFields {

        std::string rss_type;
    };



    class Names {
    public:
        std::string ru;
        std::string en;
        std::string alternative;

        Names(nlohmann::json obj) {
          if (obj.contains("ru")) ru = obj["ru"];
          if (obj.contains("en")) en = obj["en"];
          if (obj.contains("alternative")) alternative = obj["alternative"];
        }
        Names() {};

    };

    class Poster {
    public:
        std::string url;
        std::string raw_base64_file;
        int updated_timestamp;

        Poster(nlohmann::json obj) {
          if (obj.contains("url")) url = obj["url"];
          if (obj.contains("raw_base64_file")) raw_base64_file = obj["raw_base64_file"];
          if (obj.contains("updated_timestamp")) updated_timestamp = obj["updated_timestamp"];
        }
        Poster() {};

    };

    class Series {
    public:
        std::string string;
        int first;
        int last;
        Series(nlohmann::json obj) {
          if (obj.contains("string")) string = obj["string"];
          if (obj.contains("first")) first = obj["first"];
          if (obj.contains("last")) last = obj["last"];
        }
        Series() {};
    };

    class Status {
    public:
        std::string string;
        int code;

        Status(nlohmann::json obj) {
          if (obj.contains("string")) string = obj["string"];
          if (obj.contains("code")) code = obj["code"];
        }
        Status() {};

    };

    class Type {
    public:
        std::string full_string;
        std::string string;
        std::string length;
        int series;
        int code;

        Type(nlohmann::json obj) {
          if (obj.contains("full_string")) full_string = obj["full_string"];
          if (obj.contains("string")) string = obj["string"];
          if (obj.contains("length")) length = obj["length"];
          if (obj.contains("series")) series = obj["series"];
          if (obj.contains("code")) code = obj["code"];
        }
        Type() {};

    };

    class Team {
    public:
        std::vector<std::string> voice;
        std::vector<std::string> translator;
        std::vector<std::string> editing;
        std::vector<std::string> decor;
        std::vector<std::string> timing;

        Team(nlohmann::json obj) {
          if (obj.contains("voice")) voice = obj["voice"].get<std::vector<std::string>>();
          if (obj.contains("translator")) translator = obj["translator"].get<std::vector<std::string>>();
          if (obj.contains("editing")) editing = obj["editing"].get<std::vector<std::string>>();
          if (obj.contains("decor")) decor = obj["decor"].get<std::vector<std::string>>();
          if (obj.contains("timing")) timing = obj["timing"].get<std::vector<std::string>>();
        }
        Team() {};

    };

    class Season {
    public:
        int year;
        int week_day;

        Season(nlohmann::json obj) {
          if (obj.contains("year")) year = obj["year"];
          if (obj.contains("week_day")) week_day = obj["week_day"];
        }
        Season() {};

    };

    class Blocked {
    public:
        bool blocked;
        bool bakanim;

        Blocked(nlohmann::json obj) {
          if (obj.contains("blocked")) blocked = obj["blocked"];
          if (obj.contains("bakanim")) bakanim = obj["bakanim"];
        }
        Blocked() {};

    };

    class Hsl {
    public:
        std::string fhd;
        std::string hd;
        std::string sd;

        Hsl(nlohmann::json obj) {
          if (obj.contains("fhd")) fhd = obj["fhd"];
          if (obj.contains("hd")) hd = obj["hd"];
          if (obj.contains("sd")) sd = obj["sd"];
        };
        Hsl() {};

    };

    class Playlist {
    public:
        Hsl hsl;
        int serie;
        int created_timestamp;

        Playlist(nlohmann::json obj) {
          if (obj.contains("hsl")) hsl = obj["hsl"];
          if (obj.contains("serie")) serie = obj["serie"];
          if (obj.contains("created_timestamp")) created_timestamp = obj["created_timestamp"];
        }
        Playlist() {};

    };

    class Player {
    public:
       std::string alternative_player;
       std::string host;
       Playlist playlist;

       Player(nlohmann::json obj) {
         if (obj.contains("alternative_player")) alternative_player = obj["alternative_player"];
         if (obj.contains("host")) host = obj["host"];
         if (obj.contains("playlist")) playlist = obj["playlist"];
       }
       Player() {};


    };

    class TorrentQuality {
    public:
        std::string string;
        std::string type;
        std::string encoder;
        int resolution;
        bool lq_audio;

        TorrentQuality(nlohmann::json obj) {
          if (obj.contains("string")) string = obj["string"];
          if (obj.contains("type")) type = obj["type"];
          if (obj.contains("encoder")) encoder = obj["encoder"];
          if (obj.contains("resolution")) resolution = obj["resolution"];
          if (obj.contains("lq_audio")) lq_audio = obj["lq_audio"];
        }
        TorrentQuality() {};

    };

    class TorrentFilesList {
    public:
        std::string file;
        int size;
        int offset;

        TorrentFilesList(nlohmann::json obj) {
          if (obj.contains("file")) file = obj["file"];
          if (obj.contains("size")) size = obj["size"];
          if (obj.contains("offset")) offset = obj["offset"];
        }
        TorrentFilesList() {};

    };

    class TorrentMetadata {
    public:
        std::string hash;
        std::string name;
        std::vector<std::string> announce;
        std::vector<TorrentFilesList> files_list;
        int created_timestamp;

        TorrentMetadata(nlohmann::json obj) {
          if (obj.contains("hash")) hash = obj["hash"];
          if (obj.contains("name")) name = obj["name"];
          if (obj.contains("announce"))  announce = obj["announce"].get<std::vector<std::string>>();
          if (obj.contains("files_list")) {
            for (auto it: obj["files_list"]) files_list.push_back(TorrentFilesList(it));
          }
          if (obj.contains("created_timestamp")) created_timestamp = obj["created_timestamp"];
        }
        TorrentMetadata() {};

    };

    class TorrentList {
    public:
        std::string url;
        std::string raw_base64_file;
        TorrentQuality quality;
        Series series;
        int torrent_id;
        int leechers;
        int seeders;
        int downloads;
        int total_size;
        int uploaded_timestamp;
        TorrentMetadata metadata;

        TorrentList(nlohmann::json obj) {
          if (obj.contains("url")) url = obj["url"];
          if (obj.contains("raw_base64_file")) raw_base64_file = obj["raw_base64_file"];
          if (obj.contains("quality")) quality = obj["quality"];
          if (obj.contains("series")) series = obj["series"];
          if (obj.contains("torrent_id")) torrent_id = obj["torrent_id"];
          if (obj.contains("leechers")) leechers = obj["leechers"];
          if (obj.contains("seeders")) seeders = obj["seeders"];
          if (obj.contains("downloads")) downloads = obj["downloads"];
          if (obj.contains("total_size")) total_size = obj["total_size"];
          if (obj.contains("metadata")) metadata = obj["metadata"];
          if (obj.contains("uploaded_timestamp")) uploaded_timestamp = obj["uploaded_timestamp"];
        }
        TorrentList() {};


    };


    class Error {
    public:
        std::string message;
        int code;

        Error(nlohmann::json obj) {
          if (obj["error"].contains("message")) message = obj["error"]["message"];
          if (obj["error"].contains("code")) code = obj["error"]["code"];
        }
        Error() {};

    };



    class Title {
    private:
        AnilibriaAPI* anilibria_api;

    public:
        std::vector<std::string> genres;
        std::string code;
        std::string description;
        Names names;
        Poster poster;
        Status status;
        Type type;
        Team team;
        Season season;
        Blocked blocked;
        Player player;
        TorrentList torrents;
        int id;
        int in_favorites;
        int updated;
        int last_change;
        int year;
        int week_day;

        Title(nlohmann::json obj, AnilibriaAPI* api) {
          if (obj.contains("genres")) genres = obj["genres"].get<std::vector<std::string>>();
          if (obj.contains("code")) code = obj["code"];
          if (obj.contains("description")) description = obj["description"];
          if (obj.contains("names")) names = obj["names"];
          if (obj.contains("poster")) poster = obj["poster"];
          if (obj.contains("status")) status = obj["status"];
          if (obj.contains("type")) type = obj["type"];
          if (obj.contains("team")) team = obj["team"];
          if (obj.contains("season")) season = obj["season"];
          if (obj.contains("blocked")) blocked = obj["blocked"];
          if (obj.contains("player")) player = obj["player"];
          if (obj.contains("torrents")) torrents = obj["torrents"];
          if (obj.contains("id")) id = obj["id"];
          if (obj.contains("updated")) updated = obj["updated"];
          if (obj.contains("last_change")) last_change = obj["last_change"];
          if (obj.contains("year")) year = obj["year"];
          if (obj.contains("week_day")) week_day = obj["week_day"];
          if (obj.contains("in_favorites")) in_favorites = obj["in_favorites"];

          anilibria_api = api;
        }
        Title() {};

        bool add_to_favorites() {
          in_favorites++;
          return anilibria_api->add_favorite(id);

        }
        bool remove_from_favorites() {
          in_favorites--;
          return anilibria_api->del_favorite(id);
        };
        Title(nlohmann::json obj) {
          if (obj.contains("genres")) genres = obj["genres"].get<std::vector<std::string>>();
          if (obj.contains("code")) code = obj["code"];
          if (obj.contains("description")) description = obj["description"];
          if (obj.contains("names")) names = obj["names"];
          if (obj.contains("poster")) poster = obj["poster"];
          if (obj.contains("status")) status = obj["status"];
          if (obj.contains("type")) type = obj["type"];
          if (obj.contains("team")) team = obj["team"];
          if (obj.contains("season")) season = obj["season"];
          if (obj.contains("blocked")) blocked = obj["blocked"];
          if (obj.contains("player")) player = obj["player"];
          if (obj.contains("torrents")) torrents = obj["torrents"];
          if (obj.contains("id")) id = obj["id"];
          if (obj.contains("updated")) updated = obj["updated"];
          if (obj.contains("last_change")) last_change = obj["last_change"];
          if (obj.contains("year")) year = obj["year"];
          if (obj.contains("week_day")) week_day = obj["week_day"];
          if (obj.contains("in_favorites")) in_favorites = obj["in_favorites"];
        }

    };

    class Schedule {
    private:
      AnilibriaAPI *api;
    public:
        std::vector<Title> list;
        int day;

        Schedule(nlohmann::json obj, AnilibriaAPI *api) {
          if (obj.contains("list")) {
            for (auto it : obj["list"]) list.push_back(Title(it, api));
          }
          if (obj.contains("day")) day = obj["day"];
        }
        Schedule() {};


    };

    class TorrentUserStat {
    public:
        std::string user;
        int downloaded;
        int uploaded;

        TorrentUserStat(nlohmann::json obj) {
          if (obj.contains("user")) user = obj["user"];
          if (obj.contains("downloaded")) downloaded = obj["downloaded"];
          if (obj.contains("uploaded")) uploaded = obj["uploaded"];
        }
        TorrentUserStat() {};

    };

    class Youtube {
    public:
        std::string title;
        std::string image;
        std::string youtube_id;
        int timestamp;

        Youtube(nlohmann::json obj) {
          if (obj.contains("title")) title = obj["title"];
          if (obj.contains("image")) image = obj["image"];
          if (obj.contains("youtube_id")) youtube_id = obj["youtube_id"];
          if (obj.contains("timestamp")) timestamp = obj["timestamp"];
        }
        Youtube() {};

    };

    class Feed {
    public:
        Title title;
        Youtube youtube;

        Feed(nlohmann::json obj, AnilibriaAPI* api) {
          if (obj.contains("title")) title = Title(obj["title"], api);
          if (obj.contains("youtube")) youtube = obj["youtube"];
        }
        Feed() {};

    };




}
