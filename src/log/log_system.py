import logging
import sys
import os
from logging.handlers import RotatingFileHandler

class LogSystem:
    """
    A singleton class for a global logging system.

    Ensures that only one instance of the logger is created and provides a centralized
    configuration for logging to both the console and a rotating file.
    """
    _instance = None
    _initialized = False

    def __new__(cls, *args, **kwargs):
        if not cls._instance:
            cls._instance = super(LogSystem, cls).__new__(cls)
        return cls._instance

    def __init__(self,
                 name='pillpicking',
                 level=logging.DEBUG,
                 log_dir='logs',  # New parameter for the directory
                 log_file='app.log',
                 max_bytes=10*1024*1024,  # 10 MB
                 backup_count=5):
        """
        Initializes the logging system. This only runs once.

        Args:
            name (str): The name of the logger.
            level (int): The minimum logging level (e.g., logging.DEBUG).
            log_dir (str): The directory where log files will be stored.
            log_file (str): The name of the log file.
            max_bytes (int): The maximum size of the log file before rotation.
            backup_count (int): The number of backup log files to keep.
        """
        if self._initialized:
            return

        # 1. Create the logger
        self.logger = logging.getLogger(name)
        self.logger.setLevel(level)
        
        # Prevent adding handlers multiple times
        if self.logger.hasHandlers():
            self.logger.handlers.clear()

        # 2. Define the log format
        log_format = logging.Formatter(
            '%(asctime)s - %(levelname)s - %(module)s:%(lineno)d - %(message)s'
        )

        # 3. Create a console handler (always add this first)
        console_handler = logging.StreamHandler(sys.stdout)
        console_handler.setFormatter(log_format)
        self.logger.addHandler(console_handler)

        # 4. Create the log directory if it doesn't exist
        try:
            os.makedirs(log_dir, exist_ok=True)
            log_file_path = os.path.join(log_dir, log_file)
            
            # 5. Create a rotating file handler
            file_handler = RotatingFileHandler(
                log_file_path,
                maxBytes=max_bytes,
                backupCount=backup_count,
                encoding='utf-8'
            )
            file_handler.setFormatter(log_format)
            self.logger.addHandler(file_handler)

        except (IOError, OSError) as e:
            self.logger.error(f"Failed to create file handler for directory '{log_dir}': {e}", exc_info=True)
            # Continue with just the console logger if file logging fails

        # 6. Set the initialized flag to True
        self._initialized = True
        self.logger.info("Logging system initialized.")

    def get_logger(self):
        """Returns the configured logger instance."""
        return self.logger