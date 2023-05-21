from flask import Flask


app = Flask(__name__)


@app.route("/")
def home():
    return app.send_static_file("wasm_client.html")


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
