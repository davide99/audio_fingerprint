from flask import Flask, render_template
from subs_converter import convert

app = Flask(__name__)
lyrics_file = 'i_bet.lrc'


@app.route("/lyrics/<int:song_id>")
def lyrics(song_id: int):
    return {
        'song_id': song_id,
        'lyrics': convert(lyrics_file)
    }


@app.route("/")
def home():
    return render_template("subs.html")


if __name__ == "__main__":
    app.run(debug=True)
