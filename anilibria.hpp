#ifndef AniAPI_LIBHPP
#define AniAPI_LIBHPP

#include <string>
#include <map>
#include <utility>
#include <iostream>
#include <vector>
#include <cinttypes>
#include <curl/curl.h>
#include <stdexcept>
#include "json.hpp"


std::string curl_build_query(CURL *curl, const std::map<std::string,
                             std::string> &query);


namespace AniAPI {
    typedef std::vector<std::string> strings_list_t;

    class JSONProvider {
    public:
        nlohmann::json json;

        explicit
        JSONProvider(const std::string &js) : json(std::move(nlohmann::json::parse(js))) {

        }

        explicit
        JSONProvider(nlohmann::json js) : json(std::move(js)) {

        }

        template <typename T, typename KT>
        T get(const KT &key, const T &default_value) {
            if (json.template contains(key)) {
                try {
                    return json[key].template get<T>();
                } catch (nlohmann::detail::type_error &e) {
                    return default_value;
                }
            }

            return default_value;
        }

        template <typename KT>
        JSONProvider operator[](const KT &key) {
            auto &value = json[key];

            return JSONProvider(value);
        }
    };


    class TranslationNames {
    public:
        std::string ru;
        std::string en;
        std::string alternative;

        TranslationNames() = default;
        TranslationNames(std::string _ru,
                         std::string _en,
                         std::string _alternative) : ru(std::move(_ru)),
                         en(std::move(_en)),
                         alternative(std::move(_alternative)) {}
    };

    class Poster {
    public:
        std::string url;
        uint32_t timestamp{};

        Poster() = default;
        Poster(std::string _url, uint32_t _ts) : url(std::move(_url)),
        timestamp(_ts) {}
    };

    // ts - timestamp

    class TitleType {
    public:
        std::string length;
        std::string release_type;
        std::string full_release;

        uint32_t series_count;

        TitleType() = default;
        TitleType(std::string _length, std::string _release_type,
                  std::string _full_release, uint32_t _series_count) : length(std::move(_length)), release_type(std::move(_release_type)),
                                               full_release(std::move(_full_release)), series_count(_series_count)
        {}
    };


    class TranslationTeam {
    public:
        strings_list_t voices;
        strings_list_t translators;
        strings_list_t timings;
        strings_list_t decors;
        strings_list_t editors;

        TranslationTeam() = default;
        TranslationTeam(strings_list_t _voices, strings_list_t _translators,
                        strings_list_t _timings, strings_list_t _decors,
                        strings_list_t _editors) : voices(std::move(_voices)), translators(std::move(_translators)),
                                                             timings(std::move(_timings)), decors(std::move(_decors)),
                                                             editors(std::move(_editors))
                        {}
        static TranslationTeam from_json(JSONProvider &json) {
            return TranslationTeam(json["team"].get<strings_list_t>("voice", strings_list_t{}), json["team"].get<strings_list_t>("translator", strings_list_t{}),
                                   json["team"].get<strings_list_t>("timing", strings_list_t{}), json["team"].get<strings_list_t>("decor", strings_list_t{}),
                                   json["team"].get<strings_list_t>("editing", strings_list_t{}));
        }
    };

    class TitleSeason {
    public:
        std::string season;
        uint16_t year{};
        uint8_t weekday{};

        TitleSeason() = default;
        TitleSeason(std::string _season, uint16_t _year,
                    uint8_t _weekday) : season(std::move(_season)), year(_year),
                                        weekday(_weekday)
                    {}
    };

    class TitleBlocked {
    public:
        bool blocked;
        bool bakanim;

        TitleBlocked() = default;
        TitleBlocked(bool _blocked, bool _bakanim) : blocked(_blocked), bakanim(_bakanim)
        {}
    };


    class HLSStream {
    public:
        std::string sd;
        std::string hd;
        std::string fhd;

        HLSStream() = default;
        HLSStream(std::string _sd, std::string _hd,
                  std::string _fhd) : sd(std::move(_sd)), hd(std::move(_hd)),
                                      fhd(std::move(_fhd))
                  {}
    };

    class TitlePlaylistEntry {
    public:
        HLSStream hls;

        uint32_t created_ts{};
        uint32_t id{};

        TitlePlaylistEntry() = default;
        TitlePlaylistEntry(HLSStream _hls, uint32_t _created_ts,
                           uint32_t _id) : hls(_hls), created_ts(_created_ts),
                                           id(_id)
        {}
    };

    class TitleSeries {
    public:
        std::string series;

        uint32_t first;
        uint32_t last;

        TitleSeries() = default;
        TitleSeries(std::string _series, uint32_t _first,
                    uint32_t _last) : series(std::move(_series)), first(_first),
                                      last(_last)
        {}
    };

    class TitlePlayer {
    public:
        TitleSeries series;

        std::map<std::string, TitlePlaylistEntry> playlist;
        std::string host;
        std::string alternative_player;

        TitlePlayer() = default;
        TitlePlayer(TitleSeries _series, std::map<std::string, TitlePlaylistEntry> _playlist,
                    std::string _host, std::string _alternative_player) : series(std::move(_series)), playlist(std::move(_playlist)),
                                                                          host(std::move(_host)), alternative_player(std::move(_alternative_player))
        {}
        static TitlePlayer from_json(JSONProvider &json) {
            auto playlist = json["playlist"];
            auto series = playlist["series"];

            std::map<std::string, TitlePlaylistEntry> _hls;

            for (auto &item : playlist.json.items()) {
                auto playlist_entry = JSONProvider(item.value());
                auto hls = playlist_entry["hls"];


                HLSStream hls_entry(hls.get<std::string>("sd", ""),
                                    hls.get<std::string>("hd", ""),
                                    hls.get<std::string>("fhd", ""));

                TitlePlaylistEntry _playlist_entry(hls_entry, playlist_entry.get<uint32_t>("created_timestamp", 0),
                                                  playlist_entry.get<uint32_t>("id", 0));

                _hls[item.key()] = _playlist_entry;
            }

            TitleSeries _series(series.get<std::string>("string", ""),
                                series.get<uint32_t>("first", 0),
                                series.get<uint32_t>("last", 0));

            TitlePlayer titleplayer(_series, _hls,
                                    playlist.get<std::string>("host", ""),
                                    playlist.get<std::string>("alternative_player", ""));

            return titleplayer;
        }

    };


    class Title {
    public:
        TranslationNames names;
        Poster poster;
        TitleType type;
        TranslationTeam team;
        TitleSeason season;
        TitleBlocked blocked;
        TitlePlayer player;

        std::string desc;


        strings_list_t genres;

        uint64_t in_favs{};

        int32_t id{};
        uint32_t updated_ts{};

        uint8_t status_code{};

        Title() = default;
        Title(TranslationNames _names, Poster _poster,
              TitleType _type, TranslationTeam _team,
              TitleSeason _season, TitleBlocked _blocked,
              TitlePlayer _player, std::string _desc,
              strings_list_t _genres, uint64_t _in_favs,
              int32_t _id, uint32_t _updated_ts,
              uint8_t _status_code) : names(std::move(_names)), poster(std::move(_poster)),
                                      type(std::move(_type)), team(std::move(_team)),
                                      season(std::move(_season)), blocked(_blocked),
                                      player(std::move(_player)), desc(std::move(_desc)),
                                      genres(std::move(_genres)), in_favs(_in_favs),
                                      id(_id), updated_ts(_updated_ts),
                                      status_code(_status_code)

        {}
        static Title from_json(JSONProvider &json) {
            auto names = json["names"];
            auto poster = json["poster"];
            auto type = json["type"];
            auto team = json["team"];
            auto season = json["season"];
            auto blocked = json["blocked"];
            auto player = json["player"];

            TranslationNames _names(names.get<std::string>("ru", ""),
                                    names.get<std::string>("en", ""),
                                    names.get<std::string>("alternative", ""));

            Poster _poster(poster.get<std::string>("url", ""),
                           poster.get<uint32_t>("updated_timestamp", 0));

            TitleType _type(type.get<std::string>("length", ""),
                            type.get<std::string>("string", ""),
                            type.get<std::string>("full_string", ""),
                            type.get<uint32_t>("series", 0));

            TranslationTeam _team(team.get<strings_list_t>("voice", strings_list_t{}),
                                  team.get<strings_list_t>("translator", strings_list_t{}),
                                  team.get<strings_list_t>("timing", strings_list_t{}),
                                  team.get<strings_list_t>("decor", strings_list_t{}),
                                  team.get<strings_list_t>("edit", strings_list_t{}));

            TitleSeason _season(season.get<std::string>("string", ""),
                                season.get<uint16_t>("year", 0),
                                season.get<uint8_t>("week_day", 0));

            TitleBlocked _blocked(
                    blocked.get<bool>("blocked", false),
                    blocked.get<bool>("bakanim", false));

            TitlePlayer _player = TitlePlayer::from_json(player);

            Title title(_names, _poster,
                        _type, _team,
                        _season, _blocked,
                        _player, json.get<std::string>("description", ""),
                        json.get<strings_list_t>("genres", strings_list_t{}),
                        json.get<uint64_t>("in_favorites", 0),
                        json.get<uint32_t>("id", 0),
                        json.get<uint32_t>("updates_timestamp", 0),
                        json["status"].get<uint8_t>("code", 0));
            return title;
        }


    };

    class Schedule{
    public:
        std::vector<Title> list;
        uint8_t day;

        Schedule() = default;

        Schedule(uint8_t _day, std::vector<Title> _list) : list(std::move(_list)), day(_day)
        {}

        static Schedule from_json(JSONProvider &json) {
            std::vector<Title> _list;


            for (auto &x : json["list"].json) {
                JSONProvider js(x);
                // std::cout << x.dump() << '\n';
                _list.push_back(Title::from_json(js));
            }

            Schedule schedule(json.get<uint8_t>("day", 0), _list);
            return schedule;
        }

    };



    class Anilibria {
    public:
        JSONProvider libria_method(const std::string &api_ver, const std::string &method);
        JSONProvider libria_method(const std::string &api_ver, const std::string &method,
                                     const std::map<std::string, std::string> &query);

        Title get_title(uint32_t id, const strings_list_t &filter = {},
                        const strings_list_t &remove = {});
        std::vector<Title> get_titles(const std::string &id_list, const strings_list_t &filter = {},
                                      const strings_list_t &remove = {});
        std::vector<Title> get_changes(uint32_t limit = 5, uint32_t since = 0,
                                       uint32_t after = 0, const strings_list_t &filter = {},
                                       const strings_list_t &remove = {});
        std::vector<Schedule> get_schedule(const std::string &days, const strings_list_t &filter = {},
                                           const strings_list_t &remove = {});
        strings_list_t get_caching_nodes();
        Title get_random_title(const strings_list_t &filter = {}, const strings_list_t &remove = {});
        std::vector<uint32_t> get_years();
        strings_list_t get_genres(uint8_t sorting_type = 0);
        std::vector<Title> search_titles(const std::string &search = "", uint32_t limit = 5,
                                         uint32_t after = 0, const strings_list_t &year = {},
                                         const strings_list_t &season_code = {}, const strings_list_t &genres = {},
                                         const strings_list_t &voice = {}, const strings_list_t &translator = {},
                                         const strings_list_t &editing = {}, const strings_list_t &decor = {},
                                         const strings_list_t &timing = {}, const strings_list_t &filter = {},
                                         const strings_list_t &remove = {});
        std::vector<Title> advanced_search(const std::string &query, uint32_t limit = 5,
                                           uint32_t after = 0, const std::string &order_by = "",
                                           uint8_t sort_direction = 0, const strings_list_t &filter = {},
                                           const strings_list_t &remove = {});
        TranslationTeam get_team();

    private:
        std::string host_url = "wwnd.space";
    };

}



#endif
