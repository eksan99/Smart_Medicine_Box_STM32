"""
Main application class for PillPicking system with touch screen configuration.
"""


from PySide6.QtWidgets import (QApplication, QMainWindow, QStackedWidget, 
                               QVBoxLayout, QHBoxLayout, QWidget, QLabel)
from PySide6.QtCore import Qt
from PySide6.QtGui import QFont
from typing import Dict

from src.log.log_system import LogSystem
from .touch_components import TouchButton


class PillPickingApplication(QApplication):
    """Main application class with touch screen optimizations."""
    
    def __init__(self, argv):
        super().__init__(argv)
        self.logger = LogSystem().get_logger()
        
        
        self.setup_touch_screen_config()
        self.setup_fonts()
        
    def setup_touch_screen_config(self):
        """Configure application for touch screen usage."""
        # Enable touch events
        self.setAttribute(Qt.AA_SynthesizeTouchForUnhandledMouseEvents, True)
        self.setAttribute(Qt.AA_SynthesizeMouseForUnhandledTouchEvents, True)
        
        # Set application properties for touch interface
        self.setApplicationName("PillPicking")
        self.setApplicationVersion("1.0")
        self.setOrganizationName("PillPicking")
        
        # Configure for fullscreen touch interface
        self.setQuitOnLastWindowClosed(True)
    
    def setup_fonts(self):
        """Setup fonts optimized for touch screen readability."""
        # Set default font size larger for touch screens
        font = QFont()
        font.setPointSize(14)  # Larger default font for touch screens
        self.setFont(font)

class MainWindow(QMainWindow):
    """Main window class with integrated navigation and business logic."""
    
    def __init__(self):
        super().__init__()
        
        self.logger = LogSystem().get_logger()
        
        # Initialize UI components
        self.widgets: Dict[str, QWidget] = {}
        
        self.setup_window()
        self.setup_navigation()
        self.setup_widgets()
        self.setup_connections()
        self.initialize_system()
        self.restore_navigation_state()
        
    def setup_window(self):
        """Configure main window for touch screen interface."""
        self.setWindowTitle("PillPicking System")
        
        # Set window to fullscreen for touch screen devices
        self.showFullScreen()
        
        # Remove window decorations for clean mobile-style interface
        self.setWindowFlags(Qt.FramelessWindowHint)
        
        # Set minimum size for touch interactions
        self.setMinimumSize(800, 600)
        
        # Setup main container with navigation
        main_container = QWidget()
        self.setCentralWidget(main_container)
        
        container_layout = QVBoxLayout(main_container)
        container_layout.setContentsMargins(0, 0, 0, 0)
        container_layout.setSpacing(0)
        
        # Setup navigation bar
        self.setup_navigation_bar(container_layout)
        
        # Setup central widget as stacked widget for navigation
        self.central_stack = QStackedWidget()
        container_layout.addWidget(self.central_stack)
        
    def setup_navigation_bar(self, layout):
        """Setup navigation bar with back button."""
        nav_widget = QWidget()
        nav_widget.setFixedHeight(60)
        nav_widget.setStyleSheet("""
            QWidget {
                background-color: #2196F3;
                border-bottom: 2px solid #1976D2;
            }
        """)
        
        nav_layout = QHBoxLayout(nav_widget)
        nav_layout.setContentsMargins(15, 10, 15, 10)
        
        # Back button
        self.back_button = TouchButton("← Back")
        self.back_button.setFixedSize(100, 40)
        self.back_button.setStyleSheet("""
            TouchButton {
                background-color: #1976D2;
                color: white;
                border: none;
                border-radius: 5px;
                font-size: 14px;
                font-weight: bold;
            }
            TouchButton:pressed {
                background-color: #1565C0;
            }
        """)
        self.back_button.clicked.connect(self.navigate_back)
        nav_layout.addWidget(self.back_button)
        
        # Title label
        self.nav_title = QLabel("PillPicking System")
        self.nav_title.setStyleSheet("""
            QLabel {
                color: white;
                font-size: 18px;
                font-weight: bold;
            }
        """)
        self.nav_title.setAlignment(Qt.AlignCenter)
        nav_layout.addWidget(self.nav_title, 1)
        
        # Home button
        self.home_button = TouchButton("🏠 Home")
        self.home_button.setFixedSize(100, 40)
        self.home_button.setStyleSheet("""
            TouchButton {
                background-color: #1976D2;
                color: white;
                border: none;
                border-radius: 5px;
                font-size: 14px;
                font-weight: bold;
            }
            TouchButton:pressed {
                background-color: #1565C0;
            }
        """)
        self.home_button.clicked.connect(self.navigate_to_home)
        nav_layout.addWidget(self.home_button)
        
        layout.addWidget(nav_widget)
        
    def setup_navigation(self):
        """Setup mobile-style navigation system."""
        # Navigation history for back button functionality
        self.navigation_history = []
        self.current_screen_index = 0
        
    def setup_widgets(self):
        """Initialize and setup all UI widgets."""
        # Main menu widget
        self.widgets['main_menu'] = QWidget(windowTitle="Main Menu")
        
        # Medication management widget - connected to MedicationManager
        self.widgets['medication_management'] = QWidget(windowTitle="Medication Management")
        # Schedule management widget - connected to both managers
        self.widgets['schedule_management'] = QWidget(windowTitle="Schedule Management")
        
        # Manual dispensing widget - connected to managers and hardware
        self.widgets['manual_dispensing'] = QWidget(windowTitle="Manual Dispensing")
        
        # Add all widgets to stack
        for _, widget in self.widgets.items():
            self.central_stack.addWidget(widget)
            
        # Set main menu as initial screen
        self.central_stack.setCurrentWidget(self.widgets['main_menu'])
        self.update_navigation_title("Main Menu")
        
    def setup_connections(self):
        """Setup signal connections between widgets and business logic."""
        # Main menu navigation connections
        main_menu = self.widgets['main_menu']
            
    def initialize_system(self):
        """Initialize system components and check status."""
        try:
            self.logger.info("System initialized successfully")
            
        except Exception as e:
            self.logger.error(f"System initialization failed: {e}")
            # Update status to show errors
            main_menu = self.widgets['main_menu']
            
        
    def navigate_to_screen(self, widget_name: str, title: str = ""):
        """Navigate to a specific screen by name."""
        if widget_name not in self.widgets:
            self.logger.error(f"Widget '{widget_name}' not found")
            return
            
        # Add current screen to history if not main menu
        current_widget = self.central_stack.currentWidget()
        if current_widget != self.widgets['main_menu']:
            current_index = self.central_stack.currentIndex()
            if not self.navigation_history or self.navigation_history[-1] != current_index:
                self.navigation_history.append(current_index)
        
        # Navigate to new screen
        widget = self.widgets[widget_name]
        self.central_stack.setCurrentWidget(widget)
        self.update_navigation_title(title or widget_name.replace('_', ' ').title())
        
        # Save navigation state
        self.save_navigation_state()
        
        # Refresh widget data if it has a refresh method
        if hasattr(widget, 'refresh_data'):
            widget.refresh_data()
            
    def navigate_back(self) -> bool:
        """Navigate back to previous screen."""
        if self.navigation_history:
            previous_index = self.navigation_history.pop()
            self.central_stack.setCurrentIndex(previous_index)
            
            # Update title based on current widget
            current_widget = self.central_stack.currentWidget()
            if current_widget == self.widgets['main_menu']:
                self.update_navigation_title("Main Menu")
            else:
                # Find widget name
                for name, widget in self.widgets.items():
                    if widget == current_widget:
                        self.update_navigation_title(name.replace('_', ' ').title())
                        break
            return True
        else:
            # Navigate to main menu if no history
            self.navigate_to_home()
            return True
            
    def navigate_to_home(self):
        """Navigate to main menu and clear history."""
        self.central_stack.setCurrentWidget(self.widgets['main_menu'])
        self.navigation_history.clear()
        self.update_navigation_title("Main Menu")
        
    def update_navigation_title(self, title: str):
        """Update the navigation bar title."""
        self.nav_title.setText(title)
        
        # Show/hide back button based on current screen
        is_main_menu = self.central_stack.currentWidget() == self.widgets['main_menu']
        self.back_button.setVisible(not is_main_menu)
        
    def get_current_widget(self):
        """Get currently displayed widget."""
        return self.central_stack.currentWidget()
        
    def clear_navigation_history(self):
        """Clear navigation history."""
        self.navigation_history.clear()
        
    def keyPressEvent(self, event):
        """Handle key press events for navigation."""
        # Handle back navigation with Escape key
        if event.key() == Qt.Key_Escape:
            self.navigate_back()
        else:
            super().keyPressEvent(event)
            
    def restore_navigation_state(self):
        """Restore navigation state from previous session."""
        try:
            # nav_state = self.app_state.get_navigation_state()
            # last_screen = nav_state.get("last_screen", "main_menu")
            
            # # Only restore if not main menu and widget exists
            # if last_screen != "main_menu" and last_screen in self.widgets:
            #     self.navigate_to_screen(last_screen, last_screen.replace('_', ' ').title())
            #     self.logger.info(f"Restored navigation to: {last_screen}")
            # else:
            self.navigate_to_home()
                
        except Exception as e:
            self.logger.warning(f"Failed to restore navigation state: {e}")
            self.navigate_to_home()
            
    def save_navigation_state(self):
        """Save current navigation state."""
        try:
            current_widget = self.central_stack.currentWidget()
            current_screen = "main_menu"
            
            # Find current screen name
            for name, widget in self.widgets.items():
                if widget == current_widget:
                    current_screen = name
                    break
                    
            
        except Exception as e:
            self.logger.warning(f"Failed to save navigation state: {e}")
    
    def closeEvent(self, event):
        """Handle application close event."""
        try:
            # Save navigation state
            self.save_navigation_state()
                
            self.logger.info("Application closed successfully")
            
        except Exception as e:
            self.logger.error(f"Error during application shutdown: {e}")
            
        event.accept()