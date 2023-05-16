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
    return app.send_static_file("lyrics.html")


@app.route('/<path:filename>')
def serve_static(filename):
    return app.send_static_file(filename)


@app.after_request
def add_cors_header(response):
    response.headers['Cross-Origin-Embedder-Policy'] = 'require-corp'
    response.headers['Cross-Origin-Opener-Policy'] = 'same-origin'
    return response


if __name__ == "__main__":
    app.run(debug=True)
