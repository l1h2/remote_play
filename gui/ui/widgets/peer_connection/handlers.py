import time
from typing import TYPE_CHECKING

from PyQt6.QtWidgets import QMessageBox

from utils import InterprocessMessages

if TYPE_CHECKING:
    from .peer_connection import PeerConnection


class PeerConnectionHandler:
    def __init__(self, widget: "PeerConnection"):
        """
        Initialize the handler class.

        Args:
            widget (PeerConnection): The PeerConnection widget instance.
        """
        self._widget = widget

    def handle_input(self) -> None:
        """
        Handle the input from the user.
        """
        socket_str = self._widget.ui.socket_input.text()
        if not self._widget.network.public_socket.update_from_string(socket_str):
            self._widget.ui.label.setText("Invalid IP:Port")
            return

        self._widget.ui.label.setText(
            f"Connecting to {self._widget.network.public_socket}"
        )
        self._widget.controller.stop_stun_worker()
        self._widget.controller.start_udp_peer_worker(
            self._widget.network, self._handle_output
        )

    def _handle_output(self, output: str) -> None:
        """
        Handle the output from the worker.

        Args:
            output (str): The output from the worker.
        """
        self._widget.ui.label.setText(output)

        if output != InterprocessMessages.STREAM_REQUEST.value:
            return

        self._widget.stream_handler.handle_stream_request()


class StreamHandler:
    def __init__(self, widget: "PeerConnection"):
        """
        Initialize the handler class.

        Args:
            widget (PeerConnection): The PeerConnection widget instance.
        """
        self._widget = widget

    def send_stream_request(self) -> None:
        """
        Send a stream request to the peer.
        """
        if not self._widget.controller.udp_peer_worker:
            return

        if not self._widget.controller.udp_peer_worker.send_message(
            InterprocessMessages.STREAM_REQUEST, True
        ):
            return

        if not self._wait_for_peer_response():
            self._stream_request_denied_popup()
            return

        self._start_stream_server()

    def handle_stream_request(self) -> None:
        """
        Handle the stream request from the peer.
        """
        if not self._widget.controller.udp_peer_worker:
            return

        choice = self._stream_request_popup()
        if choice != QMessageBox.StandardButton.Yes:
            self._widget.controller.udp_peer_worker.send_message(
                InterprocessMessages.STREAM_REJECT, True
            )
            return

        self._widget.controller.udp_peer_worker.send_message(
            InterprocessMessages.STREAM_ACCEPT, True
        )
        self._start_stream_client()

    def _wait_for_peer_response(self, timeout: int = 30) -> bool:
        """
        Wait for the peer's response.
        """
        if not self._widget.controller.udp_peer_worker:
            return False

        response = None
        start_time = time.time()
        while (
            response != InterprocessMessages.STREAM_ACCEPT.value
            and response != InterprocessMessages.STREAM_REJECT.value
        ):
            if time.time() - start_time > timeout:
                return False

            time.sleep(0.1)
            response = self._widget.controller.udp_peer_worker.last_output

        if response == InterprocessMessages.STREAM_REJECT.value:
            return False

        return True

    def _start_stream_server(self) -> None:
        """
        Start the stream server.
        """
        self._widget.controller.stop_udp_peer_worker()
        self._widget.controller.start_udp_server_worker(
            self._widget.network, self._handle_output
        )

    def _start_stream_client(self) -> None:
        """
        Start the stream client.
        """
        self._widget.controller.stop_udp_peer_worker()
        self._widget.controller.start_udp_client_worker(
            self._widget.network, self._handle_output
        )

    def _handle_output(self, output: str) -> None:
        """
        Handle the output from the worker.

        Args:
            output (str): The output from the worker.
        """
        self._widget.ui.label.setText(output)

    def _stream_request_popup(self) -> QMessageBox.StandardButton:
        """
        Show a popup to the user asking if they want to accept the stream request.

        Returns:
            QMessageBox.StandardButton: The button clicked by the user.
        """
        return QMessageBox.question(
            self._widget,
            "Stream Request",
            "Do you accept the stream request?",
            QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No,
        )

    def _stream_request_denied_popup(self) -> QMessageBox.StandardButton:
        """
        Show a popup to the user asking if they want to accept the stream request.

        Returns:
            QMessageBox.StandardButton: The button clicked by the user.
        """
        return QMessageBox.question(
            self._widget,
            "Stream Request",
            "Stream request denied",
            QMessageBox.StandardButton.Ok,
        )
