import subprocess
import time

from PyQt6.QtCore import QThread, pyqtSignal

from utils import InterprocessMessages, Network, Subprocess


class SubprocessNetWorker(QThread):
    """
    ### Worker that calls a subprocess to establish a network connection.

    #### Attributes:
    - `network (Network)`: The network object containing the local and public sockets.
    - `exe_path (str)`: The path to the executable.

    #### Properties:
    - `listening (bool)`: Return whether the worker is listening for process output.
    - `last_output (str)`: Return the last output from the process.

    #### Signals:
    - `output (str)`: Signal emitted when the worker receives output from the process.

    #### Methods:
    - `set_args(args: list[str]) -> None`: Set the arguments for the subprocess.
    - `run() -> None`: Overrides the `run` method in `QThread` to start the subprocess.
    - `terminate_process() -> None`: Terminate the subprocess.
    - `quit() -> None`: Overrides the `quit` method in `QThread` and terminates the subprocess.
    """

    output = pyqtSignal(str)

    def __init__(self, network: Network, exe_path: str):
        super().__init__()
        self.network = network.clone()
        self.exe_path = exe_path
        self._args: list[str] = []
        self._process = None
        self._listening = False
        self._process_output = ""

    @property
    def listening(self) -> bool:
        """
        Return whether the worker is listening for process output.
        """
        return self._listening

    @property
    def last_output(self) -> str:
        """
        Return the last output from the process.
        """
        return self._process_output

    def set_args(self, args: list[str]) -> None:
        """
        Set the arguments for the subprocess.

        Args:
            args (list[str]): The arguments for the subprocess.
        """
        self._args = args

    def run(self) -> None:
        """
        Overrides the `run` method in `QThread` to start the subprocess.
        """
        self._process = subprocess.Popen(
            [self.exe_path, *self._args],
            stdout=subprocess.PIPE,
            stdin=subprocess.PIPE,
            text=True,
            creationflags=subprocess.CREATE_NO_WINDOW,
        )

        self._listening = True

        while self._listening:
            if not self._process.stdout:
                continue

            self._process_output = self._process.stdout.readline().strip()
            print(self._process_output)
            if not self._process_output:  # Check for EOF
                break

            self._handle_process_output(self._process_output)

        self.terminate_process()

    def send_message(
        self, message: InterprocessMessages, expect_ack: bool = False, timeout: int = 10
    ) -> bool:
        """
        Send a message to the subprocess' stdin.

        Args:
            message (InterprocessMessages): The message to send.
            expect_ack (bool): Whether to expect an acknowledgment from the subprocess.
            timeout (int): The timeout for the acknowledgment.

        Returns:
            bool: Whether the message was sent successfully
        """
        if not self._process or not self._process.stdin:
            return False

        self._process.stdin.write(f"{message.value}\n")
        self._process.stdin.flush()

        if not expect_ack:
            return True

        return self._wait_for_ack(message, timeout)

    def terminate_process(self) -> None:
        """
        Terminate the subprocess.
        """
        if not self._listening:
            return

        self._listening = False

        if not self._process:
            return

        self._process.terminate()
        self._process.wait()
        self._close_pipes()
        self._process = None

    def quit(self) -> None:
        """
        Overrides the `quit` method in `QThread` and terminates the subprocess.
        """
        self.terminate_process()
        super().quit()

    def _wait_for_ack(self, message: InterprocessMessages, timeout: int = 10) -> bool:
        """
        Wait for an acknowledgment from the subprocess.

        Args:
            message (InterprocessMessages): The message to wait for an acknowledgment.
            timeout (int): The timeout for the acknowledgment.

        Returns:
            bool: Whether the acknowledgment was received.
        """
        ack = InterprocessMessages.get_ack(message)
        response = None

        start_time = time.time()
        while response != ack.value:
            if time.time() - start_time > timeout:
                return False

            time.sleep(0.1)
            response = self.last_output

        return True

    def _handle_process_output(self, process_output: str) -> None:
        """
        Handle the output from the process. This method should be overridden.

        Args:
            process_output (str): The output from the process.
        """
        raise NotImplementedError

    def _close_pipes(self):
        """
        Close the pipes for the process.
        """
        if not self._process:
            return

        if self._process.stdout:
            self._process.stdout.close()

        if self._process.stdin:
            self._process.stdin.close()


class StunQueryWorker(SubprocessNetWorker):
    """
    ### Worker that calls a subprocess to query the STUN server.

    #### Inherits from:
    - `SubprocessNetWorker`
    """

    def __init__(self, network: Network, exe_path: str = Subprocess.STUN_CLIENT):
        super().__init__(network, exe_path)
        self.set_args([str(self.network.local_port)])

    def _handle_process_output(self, process_output: str) -> None:
        """
        Handle the output from the process.

        Args:
            process_output (str): The output from the process.
        """
        if self.network.public_socket.update_from_string(process_output):
            self.output.emit(f"{self.network.public_socket}")


class UdpPeerWorker(SubprocessNetWorker):
    """
    ### Worker that calls a subprocess to establish a UDP connection.

    #### Inherits from:
    - `SubprocessNetWorker`
    """

    def __init__(self, network: Network, exe_path: str = Subprocess.UDP_CONNECTION):
        super().__init__(network, exe_path)
        self.set_args(
            ["-p", str(self.network.local_port), str(self.network.public_socket)]
        )

    def _handle_process_output(self, process_output: str) -> None:
        """
        Handle the output from the process.

        Args:
            process_output (str): The output from the process.
        """
        self.output.emit(process_output)


class UdpServerWorker(SubprocessNetWorker):
    """
    ### Worker that calls a subprocess to establish a UDP server.

    #### Inherits from:
    - `SubprocessNetWorker`
    """

    def __init__(self, network: Network, exe_path: str = Subprocess.UDP_SERVER):
        super().__init__(network, exe_path)
        self.set_args([str(self.network.local_port)])

    def _handle_process_output(self, process_output: str) -> None:
        """
        Handle the output from the process.

        Args:
            process_output (str): The output from the process.
        """
        self.output.emit(process_output)


class UdpClientWorker(SubprocessNetWorker):
    """
    ### Worker that calls a subprocess to establish a UDP client.

    #### Inherits from:
    - `SubprocessNetWorker`
    """

    def __init__(self, network: Network, exe_path: str = Subprocess.UDP_CLIENT):
        super().__init__(network, exe_path)
        self.set_args([str(self.network.local_port)])

    def _handle_process_output(self, process_output: str) -> None:
        """
        Handle the output from the process.

        Args:
            process_output (str): The output from the process.
        """
        self.output.emit(process_output)
