import socket
import time
from functools import wraps


def request_timer(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        start_time = time.perf_counter()
        result = func(*args, **kwargs)
        end_time = time.perf_counter()

        elapsed = end_time - start_time
        print(f"request took {elapsed:.4f} seconds")

        return result

    return wrapper


@request_timer
def http_request(host: str, port: int) -> None:
    http_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    http_client.connect((host, port))
    http_client.send(b"GET / HTTP/1.1\r\nHost: " + host.encode() + b"\r\n\r\n")
    response = http_client.recv(30720)
    print(response.decode("utf-8"))
    http_client.close()


if __name__ == "__main__":
    for i in range(50):
        http_request("0.0.0.0", 6969)
