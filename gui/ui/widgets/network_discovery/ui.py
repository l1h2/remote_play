from typing import TYPE_CHECKING

from PyQt6.QtWidgets import QHBoxLayout, QLabel, QPushButton

if TYPE_CHECKING:
    from .network_discovery import NetworkDiscovery


class NetworkDiscoveryUI:
    """
    ### UI class for the NetworkDiscovery widget.

    #### Attributes:
    - `label (QLabel)`: The label displaying the socket string.
    - `copy_button (QPushButton)`: The button to copy the socket string to clipboard.
    """

    def __init__(self, widget: "NetworkDiscovery"):
        """
        Initialize the UI for the NetworkDiscovery widget.

        Args:
            widget (NetworkDiscovery): The NetworkDiscovery widget instance.
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
        self._add_label()
        self._add_copy_button()

    def _add_label(self) -> None:
        """
        Add a label to the widget.
        """
        self.label = QLabel("Starting...")

    def _add_copy_button(self) -> None:
        """
        Add a copy button to the widget.
        """
        self.copy_button = QPushButton("Copy to Clipboard")
        self.copy_button.clicked.connect(self._widget.stun_handler.copy_to_clipboard)

    def _set_layout(self) -> None:
        """
        Set the layout of the widget.
        """
        network_layout = QHBoxLayout(self._widget)
        network_layout.addWidget(self.label)
        network_layout.addWidget(self.copy_button)
