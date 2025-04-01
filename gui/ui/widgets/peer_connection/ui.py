from typing import TYPE_CHECKING

from PyQt6.QtWidgets import QHBoxLayout, QLabel, QLineEdit, QPushButton, QVBoxLayout

if TYPE_CHECKING:
    from .peer_connection import PeerConnection


class PeerConnectionUI:
    """
    ### UI class for the PeerConnection widget.

    #### Attributes:
    - `socket_input (QLineEdit)`: Input field for the socket string.
    - `connect_button (QPushButton)`: Button to establish a connection.
    - `label (QLabel)`: Label displaying the socket string.
    - `stream_request_button (QPushButton)`: Button to request a stream.
    """

    def __init__(self, widget: "PeerConnection"):
        """
        Initialize the UI for the PeerConnection widget.

        Args:
            widget (PeerConnection): The PeerConnection widget instance.
        """
        self._widget = widget
        self._init_ui()

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
        self.socket_input = QLineEdit()
        self.socket_input.setFixedWidth(200)
        self.socket_input.setPlaceholderText("Enter IP:Port")
        self.socket_input.returnPressed.connect(
            self._widget.connection_handler.handle_input
        )

    def _add_connect_button(self) -> None:
        """
        Add the connect button to the widget.
        """
        self.connect_button = QPushButton("Connect")
        self.connect_button.clicked.connect(
            self._widget.connection_handler.handle_input
        )

    def _add_label(self) -> None:
        """
        Add the label to the widget.
        """
        self.label = QLabel("Enter IP:Port")

    def _add_stream_request_button(self) -> None:
        """
        Add the stream request button to the widget.
        """
        self.stream_request_button = QPushButton("Stream")
        self.stream_request_button.clicked.connect(
            self._widget.stream_handler.send_stream_request
        )

    def _set_layout(self) -> None:
        """
        Set the layout of the widget.
        """
        main_layout = QVBoxLayout(self._widget)

        connection_layout = QHBoxLayout()
        connection_layout.addWidget(self.socket_input)
        connection_layout.addWidget(self.connect_button)
        connection_layout.addWidget(self.label)

        main_layout.addLayout(connection_layout)
        main_layout.addWidget(self.stream_request_button)
