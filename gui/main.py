import os
import subprocess

from PyQt6.QtCore import Qt
from PyQt6.QtWidgets import QApplication, QLabel, QMainWindow


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Simple PyQt6 App")

        exe_path = os.path.join(os.path.dirname(__file__), "bin", "main.exe")
        result = subprocess.run([exe_path], capture_output=True, text=True)
        label_text = result.stdout.strip()

        label = QLabel(label_text)
        label.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.setCentralWidget(label)


if __name__ == "__main__":
    app = QApplication([])

    window = MainWindow()
    window.show()

    app.exec()
