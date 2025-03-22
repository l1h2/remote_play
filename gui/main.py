import os
import subprocess

from PyQt6.QtCore import Qt, QThread, pyqtSignal
from PyQt6.QtGui import QCloseEvent
from PyQt6.QtWidgets import QApplication, QLabel, QMainWindow


class Worker(QThread):
    output = pyqtSignal(str)

    def __init__(self, exe_path: str, port: int):
        super().__init__()
        self.exe_path = exe_path
        self.port = port
        self.process = None
        self._running = True

    def run(self) -> None:
        self.process = subprocess.Popen(
            [self.exe_path, str(self.port)],
            stdout=subprocess.PIPE,
            text=True,
            creationflags=subprocess.CREATE_NO_WINDOW,
        )

        while self._running:
            if not self.process.stdout:
                continue

            output = self.process.stdout.readline().strip()
            print(output)
            if not output:  # Check for EOF
                self.terminate_process()
            else:
                self.output.emit(output)

    def terminate_process(self) -> None:
        if not self._running:
            return

        self._running = False

        if not self.process:
            return

        self.process.terminate()
        self.process.wait()

        if not self.process.stdout:
            return

        self.process.stdout.close()


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Simple PyQt6 App")
        self.resize(600, 400)

        self.label = QLabel("Starting...")
        self.label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.setCentralWidget(self.label)

        self.exe_path = os.path.join(
            os.path.dirname(__file__), "bin", "stun_client.exe"
        )
        self.port = 12345
        self.worker = Worker(self.exe_path, self.port)
        self.worker.output.connect(self.update_label)
        self.worker.start()

    def update_label(self, output: str) -> None:
        self.label.setText(output)

    def closeEvent(self, a0: QCloseEvent | None) -> None:
        self.worker.terminate_process()
        self.worker.wait()
        super().closeEvent(a0)


if __name__ == "__main__":
    app = QApplication([])

    window = MainWindow()
    window.show()

    app.exec()
