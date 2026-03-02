import pty
import os
import select
import sys
import time

def main():
    pid, fd = pty.fork()

    if pid == 0:
        # Child process: execute the binary
        os.execl("./game", "./game")
    else:
        # Parent process: read from the pty
        output = b""
        start_time = time.time()
        timeout = 2.0  # seconds

        while True:
            # Check if there is data to read
            r, _, _ = select.select([fd], [], [], 0.1)
            if fd in r:
                try:
                    data = os.read(fd, 1024)
                    if not data:
                        break
                    output += data
                except OSError:
                    break

            # Send 'q' to exit the game cleanly after a brief moment
            if time.time() - start_time > 0.5:
                os.write(fd, b'q')

            if time.time() - start_time > timeout:
                print("Test timed out!")
                break

        # Check for cursor hide and show sequences
        output_str = output.decode('utf-8', errors='ignore')
        print(f"Captured output length: {len(output_str)}")

        hide_seq = "\033[?25l"
        show_seq = "\033[?25h"

        if hide_seq in output_str:
            print("SUCCESS: Found cursor hide sequence.")
        else:
            print("ERROR: Did NOT find cursor hide sequence.")
            sys.exit(1)

        if show_seq in output_str:
            print("SUCCESS: Found cursor show sequence.")
        else:
            print("ERROR: Did NOT find cursor show sequence.")
            sys.exit(1)

        print("All UX validations passed.")
        sys.exit(0)

if __name__ == "__main__":
    main()
