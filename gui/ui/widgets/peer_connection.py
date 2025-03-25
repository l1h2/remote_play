from PyQt6.QtGui import QCloseEvent
from PyQt6.QtWidgets import QHBoxLayout, QLabel, QLineEdit, QWidget

from controllers import NetworkController
from utils import Network


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
        self.controller.stop_udp_worker()
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
        self._add_input()
        self._add_label()

    def _add_input(self) -> None:
        """
        Add the input field to the widget.
        """
        self._input = QLineEdit()
        self._input.setFixedWidth(200)
        self._input.setPlaceholderText("Enter IP:Port")
        self._input.returnPressed.connect(self._handle_input)

    def _add_label(self) -> None:
        """
        Add the label to the widget.
        """
        self._label = QLabel("Enter IP:Port")

    def _set_layout(self) -> None:
        """
        Set the layout of the widget.
        """
        self._layout = QHBoxLayout(self)
        self._layout.addWidget(self._input)
        self._layout.addWidget(self._label)

    def _handle_input(self) -> None:
        """
        Handle the input from the user.
        """
        socket_str = self._input.text()
        if not self.network.public_socket.update_from_string(socket_str):
            self._label.setText("Invalid IP:Port")
            return

        self._label.setText(f"Connecting to {self.network.public_socket}")
        self.controller.stop_stun_worker()
        self.controller.start_udp_worker(self.network, self._handle_output)

    def _handle_output(self, output: str) -> None:
        """
        Handle the output from the worker.

        Args:
            output (str): The output from the worker.
        """
        self._label.setText(output)
