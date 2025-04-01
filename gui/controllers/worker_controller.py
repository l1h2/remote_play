from collections.abc import Callable

from utils import Network
from workers import SubprocessNetWorker, SubprocessNetWorkerFactory


class WorkerController:
    """
    ### Controller for managing a worker thread.

    #### Properties:
    - `worker (SubprocessNetWorker | None)`: The current worker instance or None if not initialized.

    #### Methods:
    - `start(network: Network, callback: Callable[[str], None]) -> None`: Start the worker's subprocess if not already running.
    - `stop() -> None`: Stop the worker's subprocess if running.
    - `terminate() -> None`: Terminate the worker thread and clean up resources.
    """

    def __init__(self, worker_class: SubprocessNetWorkerFactory) -> None:
        """
        Initialize the WorkerController with a specific worker class.

        Args:
            worker_class (SubprocessNetWorkerFactory): The class of the worker to be managed.
        """
        self._worker_class = worker_class
        self._worker: SubprocessNetWorker | None = None

    @property
    def worker(self) -> SubprocessNetWorker | None:
        """
        Get the current worker instance.

        Returns:
            SubprocessNetWorker | None: The current worker instance or None if not initialized.
        """
        return self._worker

    def start(self, network: Network, callback: Callable[[str], None]) -> None:
        """
        Start the worker's subprocess if it is not already running.

        Args:
            network (Network): The network object to be used by the worker.
            callback (Callable[[str], None]): The callback function to handle the output.
        """
        if self._worker and self._worker.listening:
            return

        self._worker = self._worker_class(network)
        self._worker.output.connect(callback)
        self._worker.start()

    def stop(self) -> None:
        """
        Stop the worker's subprocess if it is running.
        """
        if not self._worker or not self._worker.listening:
            return

        self._worker.terminate_process()
        self._worker.wait()

    def terminate(self) -> None:
        """
        Terminate the worker thread and clean up resources.
        """
        if not self._worker:
            return

        if self._worker.listening:
            self._worker.terminate_process()

        self._worker.quit()
        self._worker.wait()
        self._worker = None
