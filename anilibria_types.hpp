
#pragma once
#include <string>
#include <vector>
#include "anilibria_api.hpp"
#include <nlohmann/json.hpp>


namespace anilibria {


    struct AdditionalFields {

        std::string filter;
        std::string remove;
        std::string include;
        std::string description_type;
        std::string playlist_type;
        int limit = -1;
        int since = -1;
        int after = -1;



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
        int sort_direction = -1;

    };

    struct SeedStatsRequest : public AdditionalFields {

        std::string users;
        std::string sort_by;
        int order = -1;
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
          if (obj.contains("ru") && !obj["ru"].is_null()) ru = obj["ru"];
          if (obj.contains("en") && !obj["en"].is_null()) en = obj["en"];
          if (obj.contains("alternative") && !obj["alternative"].is_null()) alternative = obj["alternative"];
        }
        Names() {};

    };

    class Poster {
    public:
        std::string url;
        std::string raw_base64_file;
        int updated_timestamp;

        Poster(nlohmann::json obj) {
          if (obj.contains("url") && !obj["url"].is_null()) url = obj["url"];
          if (obj.contains("raw_base64_file") && !obj["raw_base64_file"].is_null()) raw_base64_file = obj["raw_base64_file"];
          if (obj.contains("updated_timestamp") && !obj["updated_timestamp"].is_null()) updated_timestamp = obj["updated_timestamp"];
        }
        Poster() {};

    };

    class Series {
    public:
        std::string string;
        int first;
        int last;
        Series(nlohmann::json obj) {
          if (obj.contains("string") && !obj["string"].is_null()) string = obj["string"];
          if (obj.contains("first") && !obj["first"].is_null()) first = obj["first"];
          if (obj.contains("last") && !obj["last"].is_null()) last = obj["last"];
        }
        Series() {};
    };

    class Status {
    public:
        std::string string;
        int code;

        Status(nlohmann::json obj) {
          if (obj.contains("string") && !obj["string"].is_null()) string = obj["string"];
          if (obj.contains("code") && !obj["code"].is_null()) code = obj["code"];
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
          if (obj.contains("full_string") && !obj["full_string"].is_null()) full_string = obj["full_string"];
          if (obj.contains("string") && !obj["string"].is_null()) string = obj["string"];
          if (obj.contains("length") && !obj["length"].is_null()) length = obj["length"];
          if (obj.contains("series") && !obj["series"].is_null()) series = obj["series"];
          if (obj.contains("code") && !obj["code"].is_null()) code = obj["code"];
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
          if (obj.contains("voice") && !obj["voice"].is_null()) voice = obj["voice"].get<std::vector<std::string>>();
          if (obj.contains("translator") && !obj["translator"].is_null()) translator = obj["translator"].get<std::vector<std::string>>();
          if (obj.contains("editing") && !obj["editing"].is_null()) editing = obj["editing"].get<std::vector<std::string>>();
          if (obj.contains("decor") && !obj["decor"].is_null()) decor = obj["decor"].get<std::vector<std::string>>();
          if (obj.contains("timing") && !obj["timing"].is_null()) timing = obj["timing"].get<std::vector<std::string>>();
        }
        Team() {};

    };

    class Season {
    public:
        int year;
        int week_day;

        Season(nlohmann::json obj) {
          if (obj.contains("year") && !obj["year"].is_null()) year = obj["year"];
          if (obj.contains("week_day") && !obj["week_day"].is_null()) week_day = obj["week_day"];
        }
        Season() {};

    };

    class Blocked {
    public:
        bool blocked;
        bool bakanim;

        Blocked(nlohmann::json obj) {
          if (obj.contains("blocked") && !obj["blocked"].is_null()) blocked = obj["blocked"];
          if (obj.contains("bakanim") && !obj["bakanim"].is_null()) bakanim = obj["bakanim"];
        }
        Blocked() {};

    };

    class Hsl {
    public:
        std::string fhd;
        std::string hd;
        std::string sd;

        Hsl(nlohmann::json obj) {
          if (obj.contains("fhd") && !obj["fhd"].is_null()) fhd = obj["fhd"];
          if (obj.contains("hd") && !obj["hd"].is_null()) hd = obj["hd"];
          if (obj.contains("sd") && !obj["sd"].is_null()) sd = obj["sd"];
        };
        Hsl() {};

    };

    class Playlist {
    public:
        Hsl hsl;
        int serie;
        int created_timestamp;

        Playlist(nlohmann::json obj) {
          if (obj.contains("hsl") && !obj["hsl"].is_null()) hsl = obj["hsl"];
          if (obj.contains("serie") && !obj["serie"].is_null()) serie = obj["serie"];
          if (obj.contains("created_timestamp") && !obj["created_timestamp"].is_null()) created_timestamp = obj["created_timestamp"];
        }
        Playlist() {};

    };

    class Player {
    public:
       std::string alternative_player;
       std::string host;
       Playlist playlist;
       Series series;

       Player(nlohmann::json obj) {
         if (obj.contains("alternative_player") && !obj["alternative_player"].is_null()) alternative_player = obj["alternative_player"];
         if (obj.contains("host") && !obj["host"].is_null()) host = obj["host"];
         if (obj.contains("playlist") && !obj["playlist"].is_null()) playlist = obj["playlist"];
         if (obj.contains("series") && !obj["series"].is_null()) series = obj["series"];
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
          if (obj.contains("string") && !obj["string"].is_null()) string = obj["string"];
          if (obj.contains("type") && !obj["type"].is_null()) type = obj["type"];
          if (obj.contains("encoder") && !obj["encoder"].is_null()) encoder = obj["encoder"];
          if (obj.contains("resolution") && !obj["resolution"].is_null()) resolution = obj["resolution"];
          if (obj.contains("lq_audio") && !obj["lq_audio"].is_null()) lq_audio = obj["lq_audio"];
        }
        TorrentQuality() {};

    };

    class TorrentFilesList {
    public:
        std::string file;
        int size;
        int offset;

        TorrentFilesList(nlohmann::json obj) {
          if (obj.contains("file") && !obj["file"].is_null()) file = obj["file"];
          if (obj.contains("size") && !obj["size"].is_null()) size = obj["size"];
          if (obj.contains("offset") && !obj["offset"].is_null()) offset = obj["offset"];
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
          if (obj.contains("hash") && !obj["hash"].is_null()) hash = obj["hash"];
          if (obj.contains("name") && !obj["name"].is_null()) name = obj["name"];
          if (obj.contains("announce") && !obj["announce"].is_null())  announce = obj["announce"].get<std::vector<std::string>>();
          if (obj.contains("files_list") && !obj["files_list"].is_null()) {
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
          if (obj.contains("url") && !obj["url"].is_null()) url = obj["url"];
          if (obj.contains("raw_base64_file") && !obj["raw_base64_file"].is_null()) raw_base64_file = obj["raw_base64_file"];
          if (obj.contains("quality") && !obj["quality"].is_null()) quality = obj["quality"];
          if (obj.contains("series") && !obj["series"].is_null()) series = obj["series"];
          if (obj.contains("torrent_id") && !obj["torrent_id"].is_null()) torrent_id = obj["torrent_id"];
          if (obj.contains("leechers") && !obj["leechers"].is_null()) leechers = obj["leechers"];
          if (obj.contains("seeders") && !obj["seeders"].is_null()) seeders = obj["seeders"];
          if (obj.contains("downloads") && !obj["downloads"].is_null()) downloads = obj["downloads"];
          if (obj.contains("total_size") && !obj["total_size"].is_null()) total_size = obj["total_size"];
          if (obj.contains("metadata") && !obj["metadata"].is_null()) metadata = obj["metadata"];
          if (obj.contains("uploaded_timestamp") && !obj["uploaded_timestamp"].is_null()) uploaded_timestamp = obj["uploaded_timestamp"];
        }
        TorrentList() {};


    };


    class Error {
    public:
        std::string message;
        int code;

        Error(nlohmann::json obj) {
          if (obj["error"].contains("message") && !obj["message"].is_null()) message = obj["error"]["message"];
          if (obj["error"].contains("code") && !obj["message"].is_null()) code = obj["error"]["code"];
        }
        Error() {};

    };



    class Title {
    // private:
    //     AnilibriaAPI* anilibria_api;

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
        int id = -1;
        int in_favorites;
        int updated;
        int last_change;
        int year;
        int week_day;

        // Title(nlohmann::json obj, AnilibriaAPI* api) {
        //   if (obj.contains("genres")) genres = obj["genres"].get<std::vector<std::string>>();
        //   if (obj.contains("code")) code = obj["code"];
        //   if (obj.contains("description")) description = obj["description"];
        //   if (obj.contains("names")) names = obj["names"];
        //   if (obj.contains("poster")) poster = obj["poster"];
        //   if (obj.contains("status")) status = obj["status"];
        //   if (obj.contains("type")) type = obj["type"];
        //   if (obj.contains("team")) team = obj["team"];
        //   if (obj.contains("season")) season = obj["season"];
        //   if (obj.contains("blocked")) blocked = obj["blocked"];
        //   if (obj.contains("player")) player = obj["player"];
        //   if (obj.contains("torrents")) torrents = obj["torrents"];
        //   if (obj.contains("id")) id = obj["id"];
        //   if (obj.contains("updated")) updated = obj["updated"];
        //   if (obj.contains("last_change")) last_change = obj["last_change"];
        //   if (obj.contains("year")) year = obj["year"];
        //   if (obj.contains("week_day")) week_day = obj["week_day"];
        //   if (obj.contains("in_favorites")) in_favorites = obj["in_favorites"];

          // anilibria_api = api;
        // }
        Title() {};
        //TODO: make this
        // bool add_to_favorites() {
        //   in_favorites++;
        //   return anilibria_api->add_favorite(id);
        //
        // }
        // bool remove_from_favorites() {
        //   in_favorites--;
        //   return anilibria_api->del_favorite(id);
        // };
        Title(nlohmann::json obj) {
          if (obj.contains("genres") && !obj["genres"].is_null()) genres = obj["genres"].get<std::vector<std::string>>();
          if (obj.contains("code") && !obj["code"].is_null()) code = obj["code"];
          if (obj.contains("description") && !obj["description"].is_null()) description = obj["description"];
          if (obj.contains("names") && !obj["names"].is_null()) names = obj["names"];
          if (obj.contains("poster") && !obj["poster"].is_null()) poster = obj["poster"];
          if (obj.contains("status") && !obj["status"].is_null()) status = obj["status"];
          if (obj.contains("type") && !obj["type"].is_null()) type = obj["type"];
          if (obj.contains("team") && !obj["team"].is_null()) team = obj["team"];
          if (obj.contains("season") && !obj["season"].is_null()) season = obj["season"];
          if (obj.contains("blocked") && !obj["blocked"].is_null()) blocked = obj["blocked"];
          if (obj.contains("player") && !obj["player"].is_null()) player = obj["player"];
          if (obj.contains("torrents") && !obj["torrents"].is_null()) torrents = obj["torrents"];
          if (obj.contains("id") && !obj["id"].is_null()) id = obj["id"];
          if (obj.contains("updated") && !obj["updated"].is_null()) updated = obj["updated"];
          if (obj.contains("last_change") && !obj["last_change"].is_null()) last_change = obj["last_change"];
          if (obj.contains("year") && !obj["year"].is_null()) year = obj["year"];
          if (obj.contains("week_day") && !obj["week_day"].is_null()) week_day = obj["week_day"];
          if (obj.contains("in_favorites") && !obj["in_favorites"].is_null()) in_favorites = obj["in_favorites"];
        }

    };

    class Schedule {
    private:
      // AnilibriaAPI *api;
    public:
        std::vector<Title> list;
        int day;

        Schedule(nlohmann::json obj) {
          if (obj.contains("list") && !obj["list"].is_null()) {
            for (auto it : obj["list"]) list.push_back(Title(it));
          }
          if (obj.contains("day") && !obj["day"].is_null()) day = obj["day"];
        }
        Schedule() {};


    };

    class TorrentUserStat {
    public:
        std::string user;
        int downloaded;
        int uploaded;

        TorrentUserStat(nlohmann::json obj) {
          if (obj.contains("user") && !obj["user"].is_null()) user = obj["user"];
          if (obj.contains("downloaded") && !obj["downloaded"].is_null()) downloaded = obj["downloaded"];
          if (obj.contains("uploaded") && !obj["uploaded"].is_null()) uploaded = obj["uploaded"];
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
          if (obj.contains("title") && !obj["title"].is_null()) title = obj["title"];
          if (obj.contains("image") && !obj["image"].is_null()) image = obj["image"];
          if (obj.contains("youtube_id") && !obj["youtube_id"].is_null()) youtube_id = obj["youtube_id"];
          if (obj.contains("timestamp") && !obj["timestamp"].is_null()) timestamp = obj["timestamp"];
        }
        Youtube() {};

    };

    class Feed {
    public:
        Title title;
        Youtube youtube;

        Feed(nlohmann::json obj) {
          if (obj.contains("title") && !obj["title"].is_null()) title = Title(obj["title"]);
          if (obj.contains("youtube") && !obj["youtube"].is_null()) youtube = obj["youtube"];
        }
        Feed() {};

    };




}
