#include "scrapers/ScraperInfos.h"

namespace mediaelch {

QString scraperInfoToTranslatedString(ShowScraperInfo info)
{
    ScraperInfoTranslation tr;
    return tr.toString(info);
}

QString scraperInfoToTranslatedString(EpisodeScraperInfo info)
{
    ScraperInfoTranslation tr;
    return tr.toString(info);
}

QSet<ShowScraperInfo> allShowScraperInfos()
{
    return {ShowScraperInfo::Actors,
        ShowScraperInfo::Banner,
        ShowScraperInfo::Certification,
        ShowScraperInfo::Fanart,
        ShowScraperInfo::FirstAired,
        ShowScraperInfo::Genres,
        ShowScraperInfo::Network,
        ShowScraperInfo::Overview,
        ShowScraperInfo::Poster,
        ShowScraperInfo::Rating,
        ShowScraperInfo::SeasonPoster,
        ShowScraperInfo::Title,
        ShowScraperInfo::Tags,
        ShowScraperInfo::ExtraArts,
        ShowScraperInfo::SeasonBackdrop,
        ShowScraperInfo::SeasonBanner,
        ShowScraperInfo::ExtraFanarts,
        ShowScraperInfo::Thumb,
        ShowScraperInfo::SeasonThumb,
        ShowScraperInfo::Runtime,
        ShowScraperInfo::Status};
}

QSet<EpisodeScraperInfo> allEpisodeScraperInfos()
{
    return {EpisodeScraperInfo::Actors,
        EpisodeScraperInfo::Certification,
        EpisodeScraperInfo::Director,
        EpisodeScraperInfo::FirstAired,
        EpisodeScraperInfo::Network,
        EpisodeScraperInfo::Overview,
        EpisodeScraperInfo::Rating,
        EpisodeScraperInfo::Thumbnail,
        EpisodeScraperInfo::Tags,
        EpisodeScraperInfo::Title,
        EpisodeScraperInfo::Writer};
}


QString scraper::movieScraperDetailToString(MovieScraperInfo detail)
{
    switch (detail) {
    case MovieScraperInfo::Invalid: return QStringLiteral("invalid");
    case MovieScraperInfo::Title: return QStringLiteral("title");
    case MovieScraperInfo::Tagline: return QStringLiteral("tagline");
    case MovieScraperInfo::Rating: return QStringLiteral("rating");
    case MovieScraperInfo::Released: return QStringLiteral("released");
    case MovieScraperInfo::Runtime: return QStringLiteral("runtime");
    case MovieScraperInfo::Certification: return QStringLiteral("certification");
    case MovieScraperInfo::Trailer: return QStringLiteral("trailer");
    case MovieScraperInfo::Overview: return QStringLiteral("overview");
    case MovieScraperInfo::Poster: return QStringLiteral("poster");
    case MovieScraperInfo::Backdrop: return QStringLiteral("backdrop");
    case MovieScraperInfo::Actors: return QStringLiteral("actors");
    case MovieScraperInfo::Genres: return QStringLiteral("genres");
    case MovieScraperInfo::Studios: return QStringLiteral("studios");
    case MovieScraperInfo::Countries: return QStringLiteral("countries");
    case MovieScraperInfo::Writer: return QStringLiteral("writer");
    case MovieScraperInfo::Director: return QStringLiteral("director");
    case MovieScraperInfo::Tags: return QStringLiteral("tags");
    case MovieScraperInfo::ExtraFanarts: return QStringLiteral("extra-fanarts");
    case MovieScraperInfo::Set: return QStringLiteral("movie-set");
    case MovieScraperInfo::Logo: return QStringLiteral("logo");
    case MovieScraperInfo::CdArt: return QStringLiteral("cd-art");
    case MovieScraperInfo::ClearArt: return QStringLiteral("clear-art");
    case MovieScraperInfo::Banner: return QStringLiteral("banner");
    case MovieScraperInfo::Thumb: return QStringLiteral("thumb");
    case MovieScraperInfo::TvShowLink: return QStringLiteral("showlink");
    }
    MediaElch_Debug_Unreachable();
    return QStringLiteral("unknown");
}

QSet<MovieScraperInfo> scraper::allMovieScraperInfos()
{
    return {MovieScraperInfo::Title,
        MovieScraperInfo::Tagline,
        MovieScraperInfo::Rating,
        MovieScraperInfo::Released,
        MovieScraperInfo::Runtime,
        MovieScraperInfo::Certification,
        MovieScraperInfo::Trailer,
        MovieScraperInfo::Overview,
        MovieScraperInfo::Poster,
        MovieScraperInfo::Backdrop,
        MovieScraperInfo::Actors,
        MovieScraperInfo::Genres,
        MovieScraperInfo::Studios,
        MovieScraperInfo::Countries,
        MovieScraperInfo::Writer,
        MovieScraperInfo::Director,
        MovieScraperInfo::Tags,
        MovieScraperInfo::ExtraFanarts,
        MovieScraperInfo::Set,
        MovieScraperInfo::Logo,
        MovieScraperInfo::CdArt,
        MovieScraperInfo::ClearArt,
        MovieScraperInfo::Banner,
        MovieScraperInfo::Thumb,
        MovieScraperInfo::TvShowLink,
        MovieScraperInfo::First};
}

QSet<MusicScraperInfo> allMusicScraperInfos()
{
    return {MusicScraperInfo::Name,
        MusicScraperInfo::Genres,
        MusicScraperInfo::Styles,
        MusicScraperInfo::Moods,
        MusicScraperInfo::YearsActive,
        MusicScraperInfo::Formed,
        MusicScraperInfo::Born,
        MusicScraperInfo::Died,
        MusicScraperInfo::Disbanded,
        MusicScraperInfo::Biography,
        MusicScraperInfo::Thumb,
        MusicScraperInfo::Fanart,
        MusicScraperInfo::Logo,
        MusicScraperInfo::Title,
        MusicScraperInfo::Artist,
        MusicScraperInfo::Review,
        MusicScraperInfo::ReleaseDate,
        MusicScraperInfo::Label,
        MusicScraperInfo::Rating,
        MusicScraperInfo::Year,
        MusicScraperInfo::CdArt,
        MusicScraperInfo::Cover,
        MusicScraperInfo::ExtraFanarts,
        MusicScraperInfo::Discography};
}

} // namespace mediaelch

ScraperInfoTranslation::~ScraperInfoTranslation() = default;

QString ScraperInfoTranslation::toString(ShowScraperInfo info)
{
    switch (info) {
    case ShowScraperInfo::Invalid: return tr("Invalid");
    case ShowScraperInfo::Actors: return tr("Actors");
    case ShowScraperInfo::Banner: return tr("Banner");
    case ShowScraperInfo::Certification: return tr("Certification");
    case ShowScraperInfo::Fanart: return tr("Fanart");
    case ShowScraperInfo::FirstAired: return tr("First Aired");
    case ShowScraperInfo::Genres: return tr("Genres");
    case ShowScraperInfo::Network: return tr("Network");
    case ShowScraperInfo::Overview: return tr("Overview");
    case ShowScraperInfo::Poster: return tr("Poster");
    case ShowScraperInfo::Rating: return tr("Rating");
    case ShowScraperInfo::SeasonPoster: return tr("Season Poster");
    case ShowScraperInfo::Title: return tr("Title");
    case ShowScraperInfo::Tags: return tr("Tags");
    case ShowScraperInfo::ExtraArts: return tr("Extra Art");
    case ShowScraperInfo::SeasonBackdrop: return tr("Season Backdrop");
    case ShowScraperInfo::SeasonBanner: return tr("Season Banner");
    case ShowScraperInfo::ExtraFanarts: return tr("Extra Fanart");
    case ShowScraperInfo::Thumb: return tr("Show Thumbnail");
    case ShowScraperInfo::SeasonThumb: return tr("Season Thumbnail");
    case ShowScraperInfo::Runtime: return tr("Runtime");
    case ShowScraperInfo::Status: return tr("Status");
    }
    MediaElch_Debug_Unreachable();
    return tr("Unknown");
}

QString ScraperInfoTranslation::toString(EpisodeScraperInfo info)
{
    switch (info) {
    case EpisodeScraperInfo::Invalid: return tr("Invalid");
    case EpisodeScraperInfo::Actors: return tr("Actors");
    case EpisodeScraperInfo::Certification: return tr("Certification");
    case EpisodeScraperInfo::Director: return tr("Director");
    case EpisodeScraperInfo::FirstAired: return tr("First Aired");
    case EpisodeScraperInfo::Network: return tr("Network");
    case EpisodeScraperInfo::Overview: return tr("Overview");
    case EpisodeScraperInfo::Rating: return tr("Rating");
    case EpisodeScraperInfo::Tags: return tr("Tags");
    case EpisodeScraperInfo::Thumbnail: return tr("Thumbnail");
    case EpisodeScraperInfo::Title: return tr("Title");
    case EpisodeScraperInfo::Writer: return tr("Writer");
    }
    MediaElch_Debug_Unreachable();
    return tr("Unknown");
}

QDebug operator<<(QDebug debug, const QSet<MovieScraperInfo>& details)
{
    for (MovieScraperInfo detail : details) {
        debug << mediaelch::scraper::movieScraperDetailToString(detail);
    }
    return debug;
}
