# test/log/test_module.py

# --- UPDATED IMPORT ---
# We now import from the 'src' package, which is visible from the project root.
from src.log.log_system import LogSystem

# Get the logger instance. The logic here remains the same.
logger = LogSystem().get_logger()

def perform_task():
    """A sample function that logs messages."""
    logger.info("Starting a task in test_module.")
    try:
        result = 10 / 0
    except ZeroDivisionError:
        logger.error("A division by zero error occurred!", exc_info=True)
    logger.info("Task finished in test_module.")