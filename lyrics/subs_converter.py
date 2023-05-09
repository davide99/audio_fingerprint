import re


def convert(lrc_file: str) -> list[dict[str, str]]:
    regex = r"\[([0-9]+):([0-9]+)\.([0-9]+)\](.+)"
    parsed_lyrics = []

    with open(lrc_file, "r", encoding="utf-8") as f:
        for line in f:
            match = re.search(regex, line)
            if match:
                minutes = int(match.group(1))
                seconds = int(match.group(2))
                millis = int(match.group(3))
                offset = minutes * 60 + seconds + millis / 100.0

                parsed_lyrics.append({
                    "offset": offset,
                    "text": match.group(4)
                })

    return parsed_lyrics
