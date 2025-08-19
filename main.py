import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).parent / "src"))

from src.ui.main_application import PillPickingApplication, MainWindow
from src.log.log_system import LogSystem

def setup_logging():
    """Setup logging configuration."""
    LogSystem(
        name='pillpicking', 
        log_dir='app_logs',
        log_file='application.log',
    )

def main():
    """Main function to run the PillPicking system."""
    setup_logging()
    logger = LogSystem().get_logger()
    
    try:
        # Create application
        app = PillPickingApplication(sys.argv)
        
        # Create main window with integrated components
        main_window = MainWindow()
        
        # Show the application
        main_window.show()
        
        logger.info("PillPicking system started")
        
        # Run the application
        exit_code = app.exec()
        
        logger.info("PillPicking system finished")
        return exit_code
        
    except Exception as e:
        logger.error(f"Failed to start PillPicking system: {e}")
        return 1

if __name__ == "__main__":
    sys.exit(main())