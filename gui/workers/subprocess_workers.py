import subprocess

from PyQt6.QtCore import QThread, pyqtSignal

from utils import Network


class SubprocessNetWorker(QThread):
    """
    ### Worker that calls a subprocess to establish a network connection.

    #### Attributes:
    - `exe_path (str)`: The path to the executable.
    - `network (Network)`: The network object containing the local and public sockets.

    #### Properties:
    - `listening (bool)`: Return whether the worker is listening for process output.

    #### Signals:
    - `output (str)`: Signal emitted when the worker receives output from the process.

    #### Methods:
    - `set_args(args: list[str]) -> None`: Set the arguments for the subprocess.
    - `run() -> None`: Overrides the `run` method in `QThread` to start the subprocess.
    - `terminate_process() -> None`: Terminate the subprocess.
    - `quit() -> None`: Overrides the `quit` method in `QThread` and terminates the subprocess.
    """

    output = pyqtSignal(str)

    def __init__(self, exe_path: str, network: Network):
        super().__init__()
        self.exe_path = exe_path
        self.network = network.clone()
        self._args: list[str] = []
        self._process = None
        self._listening = False

    @property
    def listening(self) -> bool:
        """
        Return whether the worker is listening for process output.
        """
        return self._listening

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
            text=True,
            creationflags=subprocess.CREATE_NO_WINDOW,
        )

        self._listening = True

        while self._listening:
            if not self._process.stdout:
                continue

            process_output = self._process.stdout.readline().strip()
            print(process_output)
            if not process_output:  # Check for EOF
                break

            self._handle_process_output(process_output)

        self.terminate_process()

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

        if not self._process.stdout:
            return

        self._process.stdout.close()
        self._process = None

    def quit(self) -> None:
        """
        Overrides the `quit` method in `QThread` and terminates the subprocess.
        """
        self.terminate_process()
        super().quit()

    def _handle_process_output(self, process_output: str) -> None:
        """
        Handle the output from the process. This method should be overridden.

        Args:
            process_output (str): The output from the process.
        """
        raise NotImplementedError


class StunQueryWorker(SubprocessNetWorker):
    """
    ### Worker that calls a subprocess to query the STUN server.

    #### Inherits from:
    - `SubprocessNetWorker`
    """

    def __init__(self, exe_path: str, local_port: int):
        super().__init__(exe_path, Network(local_port))
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

    def __init__(self, exe_path: str, network: Network):
        super().__init__(exe_path, network)
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
