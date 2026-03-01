import pty
import os
import select
import time

def verify_ux():
    master, slave = pty.openpty()
    pid = os.fork()

    if pid == 0:
        # Child process: run the game
        os.setsid()
        os.dup2(slave, 0)
        os.dup2(slave, 1)
        os.dup2(slave, 2)
        os.close(slave)
        os.close(master)
        os.execl('./game', './game')
    else:
        # Parent process
        os.close(slave)
        output = b''

        # Read the initial prompt and wait for it
        timeout = 2.0
        start_time = time.time()
        while time.time() - start_time < timeout:
            r, _, _ = select.select([master], [], [], 0.1)
            if r:
                data = os.read(master, 1024)
                if not data:
                    break
                output += data
            if b'Press any key to start...' in output:
                break

        # We need to send a key to start the game
        os.write(master, b' ')

        # Read until we see the first score or wait a little
        start_time = time.time()
        while time.time() - start_time < timeout:
            r, _, _ = select.select([master], [], [], 0.1)
            if r:
                data = os.read(master, 1024)
                if not data:
                    break
                output += data
            if b'Score: 0' in output:
                break

        # Send quit command
        os.write(master, b'q')

        # Read the rest of the output
        while True:
            r, _, _ = select.select([master], [], [], 0.5)
            if r:
                try:
                    data = os.read(master, 1024)
                    if not data:
                        break
                    output += data
                except OSError:
                    break
            else:
                break

        os.close(master)
        os.waitpid(pid, 0)

        output_str = output.decode('utf-8', errors='ignore')
        print("Captured output:")
        print(repr(output_str))

        assert '\033[?25l' in output_str, "Cursor hide sequence missing!"
        assert '\033[?25h' in output_str, "Cursor restore sequence missing!"
        assert 'Press any key to start...' in output_str, "Start prompt missing!"
        print("All UX checks passed!")

if __name__ == '__main__':
    verify_ux()
