import time

from PyQt6.QtGui import QCloseEvent
from PyQt6.QtWidgets import (
    QHBoxLayout,
    QLabel,
    QLineEdit,
    QMessageBox,
    QPushButton,
    QVBoxLayout,
    QWidget,
)

from controllers import NetworkController
from utils import InterprocessMessages, Network


class PeerConnection(QWidget):
    """
    Widget for establishing a connection to a peer.
    """

    def __init__(self, controller: NetworkController):
        super().__init__()
        self.controller = controller
        self.network = Network(self.controller.local_port)
        self._init_ui()

    def closeEvent(self, a0: QCloseEvent | None) -> None:
        """
        Overrides the `closeEvent` method in `QWidget`.

        Args:
            a0 (QCloseEvent | None): The close event.
        """
        self.controller.stop_udp_peer_worker()
        return super().closeEvent(a0)

    def _init_ui(self) -> None:
        """
        Initialize the widget UI.
        """
        self._add_widgets()
        self._set_layout()

    def _add_widgets(self) -> None:
        """
        Add widgets to the widget.
        """
        self._add_socket_input()
        self._add_connect_button()
        self._add_label()
        self._add_stream_request_button()

    def _add_socket_input(self) -> None:
        """
        Add the socket input field to the widget.
        """
        self._socket_input = QLineEdit()
        self._socket_input.setFixedWidth(200)
        self._socket_input.setPlaceholderText("Enter IP:Port")
        self._socket_input.returnPressed.connect(self._handle_input)

    def _add_connect_button(self) -> None:
        """
        Add the connect button to the widget.
        """
        self._connect_button = QPushButton("Connect")
        self._connect_button.clicked.connect(self._handle_input)

    def _add_label(self) -> None:
        """
        Add the label to the widget.
        """
        self._label = QLabel("Enter IP:Port")

    def _add_stream_request_button(self) -> None:
        """
        Add the stream request button to the widget.
        """
        self._stream_request_button = QPushButton("Stream")
        self._stream_request_button.clicked.connect(self._send_stream_request)

    def _set_layout(self) -> None:
        """
        Set the layout of the widget.
        """
        main_layout = QVBoxLayout(self)

        connection_layout = QHBoxLayout(self)
        connection_layout.addWidget(self._socket_input)
        connection_layout.addWidget(self._connect_button)
        connection_layout.addWidget(self._label)

        main_layout.addLayout(connection_layout)
        main_layout.addWidget(self._stream_request_button)

    def _handle_input(self) -> None:
        """
        Handle the input from the user.
        """
        socket_str = self._socket_input.text()
        if not self.network.public_socket.update_from_string(socket_str):
            self._label.setText("Invalid IP:Port")
            return

        self._label.setText(f"Connecting to {self.network.public_socket}")
        self.controller.stop_stun_worker()
        self.controller.start_udp_peer_worker(self.network, self._handle_output)

    def _handle_output(self, output: str) -> None:
        """
        Handle the output from the worker.

        Args:
            output (str): The output from the worker.
        """
        self._label.setText(output)

        if output != InterprocessMessages.STREAM_REQUEST.value:
            return

        if not self.controller.udp_peer_worker:
            return

        choice = self._stream_request_popup()
        if choice != QMessageBox.StandardButton.Yes:
            self.controller.udp_peer_worker.send_message(
                InterprocessMessages.STREAM_REJECT, True
            )
            return

        self.controller.udp_peer_worker.send_message(
            InterprocessMessages.STREAM_ACCEPT, True
        )

        # TODO: Implement the stream client

    def _stream_request_popup(self) -> QMessageBox.StandardButton:
        """
        Show a popup to the user asking if they want to accept the stream request.

        Returns:
            QMessageBox.StandardButton: The button clicked by the user.
        """
        return QMessageBox.question(
            self,
            "Stream Request",
            "Do you accept the stream request?",
            QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No,
        )

    def _send_stream_request(self) -> None:
        """
        Send a stream request to the peer.
        """
        if not self.controller.udp_peer_worker:
            return

        if not self.controller.udp_peer_worker.send_message(
            InterprocessMessages.STREAM_REQUEST, True
        ):
            print("Failed to send stream request")
            return

        if not self._wait_for_peer_response():
            print("Peer rejected the stream request")
            return

        # TODO: Implement the stream server

    def _wait_for_peer_response(self, timeout: int = 30) -> bool:
        """
        Wait for the peer's response.
        """
        if not self.controller.udp_peer_worker:
            return False

        response = None
        start_time = time.time()
        while (
            response != InterprocessMessages.ACK_STREAM_ACCEPT.value
            and response != InterprocessMessages.ACK_STREAM_REJECT.value
        ):
            if time.time() - start_time > timeout:
                return False

            time.sleep(0.1)
            response = self.controller.udp_peer_worker.last_output

        if response == InterprocessMessages.ACK_STREAM_REJECT.value:
            return False

        return True
