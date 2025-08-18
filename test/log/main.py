# test/log/main.py

import logging
import os

# --- UPDATED IMPORTS ---
from src.log.log_system import LogSystem

def main():
    """Main function to demonstrate the logging system."""
    
    # --- Define log directory and file name ---
    # This path is relative to where the script is RUN from (the project root).
    log_directory = "test_logs"
    log_filename = "application.log"
    full_log_path = os.path.join(log_directory, log_filename)

    # --- Step 2: Initialize the LogSystem ONCE ---
    LogSystem(
        name='pillpicking', 
        level=logging.DEBUG, 
        log_dir=log_directory,
        log_file=log_filename
    )

    # --- Step 3: Get the logger instance ---
    logger = LogSystem().get_logger()

    logger.info("应用程序在 main.py 中启动。")
    logger.debug("這是一條偵錯訊息，不應出現在日誌中。")
    
    # --- Step 4: Call a function from another module ---
    from test.log import test_module
    test_module.perform_task()

    logger.warning("Application is shutting down.")

    print(f"\n--- Log test complete. Check the console output and the file at: '{full_log_path}' ---")


if __name__ == "__main__":
    main()