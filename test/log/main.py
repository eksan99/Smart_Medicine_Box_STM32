# test/log/main.py
import unittest
import os
import logging
from src.log.log_system import LogSystem

class TestLogSystem(unittest.TestCase):
    def setUp(self):
        self.log_dir = "test_logs"
        self.log_file = "test_app.log"
        # Remove log file if exists
        log_path = os.path.join(self.log_dir, self.log_file)
        if os.path.exists(log_path):
            os.remove(log_path)
        if os.path.exists(self.log_dir):
            try:
                os.rmdir(self.log_dir)
            except OSError:
                pass

    def test_logsystem_initialization_and_logging(self):
        LogSystem(name="unittest", level=logging.INFO, log_dir=self.log_dir, log_file=self.log_file)
        logger = LogSystem().get_logger()
        logger.info("Test info message.")
        logger.error("Test error message.")
        log_path = os.path.join(self.log_dir, self.log_file)
        self.assertTrue(os.path.exists(log_path), "Log file was not created.")
        with open(log_path, encoding="utf-8") as f:
            log_content = f.read()
        self.assertIn("Test info message.", log_content)
        self.assertIn("Test error message.", log_content)

    def tearDown(self):
        log_path = os.path.join(self.log_dir, self.log_file)
        # Remove all handlers from the logger and close them
        logger = LogSystem().get_logger()
        handlers = logger.handlers[:]
        for handler in handlers:
            handler.close()
            logger.removeHandler(handler)
        if os.path.exists(log_path):
            os.remove(log_path)
        if os.path.exists(self.log_dir):
            try:
                os.rmdir(self.log_dir)
            except OSError:
                pass

if __name__ == "__main__":
    unittest.main()