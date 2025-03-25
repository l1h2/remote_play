from PyQt6.QtGui import QCloseEvent
from PyQt6.QtWidgets import QApplication, QHBoxLayout, QLabel, QPushButton, QWidget

from controllers import NetworkController


class NetworkDiscovery(QWidget):
    """
    ### Widget for discovering the public network socket.

    #### Attributes:
    - `controller (NetworkController)`: The network controller.

    #### Methods:
    - `closeEvent(a0: QCloseEvent | None) -> None`: Overrides the `closeEvent` method in `QWidget`.
    """

    def __init__(self, controller: NetworkController, start: bool = True):
        super().__init__()
        self.controller = controller
        self._init_ui()

        if start:
            self.controller.start_stun_worker(self._update_network)

    def closeEvent(self, a0: QCloseEvent | None) -> None:
        """
        Overrides the `closeEvent` method in `QWidget`.

        Args:
            a0 (QCloseEvent | None): The close event.
        """
        self.controller.stop_stun_worker()
        super().closeEvent(a0)

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
        self._add_label()
        self._add_copy_button()

    def _add_label(self) -> None:
        """
        Add a label to the widget.
        """
        self._label = QLabel("Starting...")

    def _add_copy_button(self) -> None:
        """
        Add a copy button to the widget.
        """
        self._copy_button = QPushButton("Copy to Clipboard")
        self._copy_button.clicked.connect(self._copy_to_clipboard)

    def _set_layout(self) -> None:
        """
        Set the layout of the widget.
        """
        self._layout = QHBoxLayout(self)
        self._layout.addWidget(self._label)
        self._layout.addWidget(self._copy_button)

    def _copy_to_clipboard(self) -> None:
        """
        Copy label text to clipboard.
        """
        clipboard = QApplication.clipboard()
        if clipboard:
            clipboard.setText(self._label.text())

    def _update_network(self, output: str) -> None:
        """
        Update the network object with socket string.

        Args:
            output (str): The socket string to update the network object with.
        """
        self._label.setText(output)
