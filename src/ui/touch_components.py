"""
Touch-friendly UI components for the PillPicking system.
"""

from PySide6.QtWidgets import (QPushButton, QLineEdit, QScrollArea, 
                               QListWidget, QListWidgetItem, QWidget, QVBoxLayout,
                               QHBoxLayout, QLabel, QSpinBox)
from PySide6.QtCore import Qt, Signal, QSize
from PySide6.QtGui import QFont


class TouchButton(QPushButton):
    """Touch-optimized button with larger size and visual feedback."""
    
    def __init__(self, text: str = "", icon=None, parent=None):
        super().__init__(text, parent)
        self.setup_touch_style()
        
    def setup_touch_style(self):
        """Configure button for touch interface."""
        # Set minimum size for touch targets
        self.setMinimumSize(120, 60)
        
        # Set font for better readability
        font = QFont()
        font.setPointSize(14)
        font.setBold(True)
        self.setFont(font)
        
        # Apply touch-friendly styling
        self.setStyleSheet("""
            TouchButton {
                background-color: #007acc;
                color: white;
                border: none;
                border-radius: 12px;
                padding: 10px 20px;
                font-size: 14px;
                font-weight: bold;
            }
            TouchButton:hover {
                background-color: #0086d9;
            }
            TouchButton:pressed {
                background-color: #005a9e;
                transform: translateY(2px);
            }
            TouchButton:disabled {
                background-color: #cccccc;
                color: #666666;
            }
        """)


class TouchLineEdit(QLineEdit):
    """Touch-optimized line edit with larger size and virtual keyboard integration."""
    
    # Signal emitted when widget gains focus (for keyboard)
    focus_gained = Signal()
    
    def __init__(self, placeholder: str = "", parent=None):
        super().__init__(parent)
        if placeholder:
            self.setPlaceholderText(placeholder)
        self.setup_touch_style()
        
    def setup_touch_style(self):
        """Configure line edit for touch interface."""
        # Set minimum height for touch targets
        self.setMinimumHeight(50)
        
        # Set font for better readability
        font = QFont()
        font.setPointSize(14)
        self.setFont(font)
        
        # Apply touch-friendly styling
        self.setStyleSheet("""
            TouchLineEdit {
                border: 2px solid #ccc;
                border-radius: 8px;
                padding: 10px 15px;
                font-size: 14px;
                background-color: white;
            }
            TouchLineEdit:focus {
                border-color: #007acc;
                background-color: #f8f9fa;
            }
        """)
        
    def focusInEvent(self, event):
        """Override focus event to emit signal for keyboard."""
        super().focusInEvent(event)
        self.focus_gained.emit()


class TouchSpinBox(QSpinBox):
    """Touch-optimized spin box with larger buttons."""
    
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setup_touch_style()
        
    def setup_touch_style(self):
        """Configure spin box for touch interface."""
        # Set minimum height for touch targets
        self.setMinimumHeight(50)
        self.setMinimumWidth(120)
        
        # Set font for better readability
        font = QFont()
        font.setPointSize(14)
        self.setFont(font)
        
        # Apply touch-friendly styling
        self.setStyleSheet("""
            TouchSpinBox {
                border: 2px solid #ccc;
                border-radius: 8px;
                padding: 5px 10px;
                font-size: 14px;
                background-color: white;
            }
            TouchSpinBox:focus {
                border-color: #007acc;
            }
            TouchSpinBox::up-button, TouchSpinBox::down-button {
                width: 30px;
                height: 25px;
                border: none;
                background-color: #f0f0f0;
            }
            TouchSpinBox::up-button:hover, TouchSpinBox::down-button:hover {
                background-color: #e0e0e0;
            }
            TouchSpinBox::up-button:pressed, TouchSpinBox::down-button:pressed {
                background-color: #d0d0d0;
            }
        """)


class TouchScrollArea(QScrollArea):
    """Touch-optimized scroll area with kinetic scrolling."""
    
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setup_touch_scrolling()
        
    def setup_touch_scrolling(self):
        """Configure scroll area for touch interface."""
        # Enable kinetic scrolling
        self.setVerticalScrollBarPolicy(Qt.ScrollBarAsNeeded)
        self.setHorizontalScrollBarPolicy(Qt.ScrollBarAsNeeded)
        
        # Style scrollbars for touch
        self.setStyleSheet("""
            TouchScrollArea {
                border: 1px solid #ddd;
                border-radius: 8px;
                background-color: white;
            }
            QScrollBar:vertical {
                width: 20px;
                background-color: #f0f0f0;
                border-radius: 10px;
            }
            QScrollBar::handle:vertical {
                background-color: #ccc;
                border-radius: 10px;
                min-height: 30px;
            }
            QScrollBar::handle:vertical:hover {
                background-color: #bbb;
            }
            QScrollBar:horizontal {
                height: 20px;
                background-color: #f0f0f0;
                border-radius: 10px;
            }
            QScrollBar::handle:horizontal {
                background-color: #ccc;
                border-radius: 10px;
                min-width: 30px;
            }
            QScrollBar::handle:horizontal:hover {
                background-color: #bbb;
            }
        """)


class TouchListWidget(QListWidget):
    """Touch-optimized list widget with larger items."""
    
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setup_touch_style()
        
    def setup_touch_style(self):
        """Configure list widget for touch interface."""
        # Set item size for touch targets
        self.setMinimumHeight(200)
        
        # Set font for better readability
        font = QFont()
        font.setPointSize(14)
        self.setFont(font)
        
        # Apply touch-friendly styling
        self.setStyleSheet("""
            TouchListWidget {
                border: 1px solid #ddd;
                border-radius: 8px;
                background-color: white;
                outline: none;
            }
            TouchListWidget::item {
                padding: 15px;
                border-bottom: 1px solid #eee;
                min-height: 40px;
            }
            TouchListWidget::item:selected {
                background-color: #007acc;
                color: white;
            }
            TouchListWidget::item:hover {
                background-color: #f0f8ff;
            }
        """)
        
    def add_touch_item(self, text: str, data=None) -> QListWidgetItem:
        """Add an item optimized for touch interaction."""
        item = QListWidgetItem(text)
        item.setSizeHint(QSize(0, 60))  # Set minimum height for touch
        
        if data is not None:
            item.setData(Qt.UserRole, data)
            
        self.addItem(item)
        return item


class MedicationListItem(QWidget):
    """Custom list item widget for medication display."""
    
    # Signals for item interactions
    edit_requested = Signal(object)  # Emits medication data
    dispense_requested = Signal(object)  # Emits medication data
    
    def __init__(self, medication_data: dict, parent=None):
        super().__init__(parent)
        self.medication_data = medication_data
        self.setup_ui()
        
    def setup_ui(self):
        """Setup the medication item UI."""
        layout = QHBoxLayout(self)
        layout.setContentsMargins(15, 10, 15, 10)
        
        # Medication info section
        info_layout = QVBoxLayout()
        
        # Medication name
        self.name_label = QLabel(self.medication_data.get('name', 'Unknown'))
        name_font = QFont()
        name_font.setPointSize(16)
        name_font.setBold(True)
        self.name_label.setFont(name_font)
        info_layout.addWidget(self.name_label)
        
        # Quantity info
        quantity = self.medication_data.get('quantity', 0)
        self.quantity_label = QLabel(f"Quantity: {quantity} pills")
        quantity_font = QFont()
        quantity_font.setPointSize(12)
        self.quantity_label.setFont(quantity_font)
        self.quantity_label.setStyleSheet("color: #666;")
        info_layout.addWidget(self.quantity_label)
        
        layout.addLayout(info_layout, 1)
        
        # Action buttons
        button_layout = QVBoxLayout()
        
        self.edit_button = TouchButton("Edit")
        self.edit_button.setMaximumSize(80, 40)
        self.edit_button.clicked.connect(lambda: self.edit_requested.emit(self.medication_data))
        button_layout.addWidget(self.edit_button)
        
        self.dispense_button = TouchButton("Dispense")
        self.dispense_button.setMaximumSize(80, 40)
        self.dispense_button.clicked.connect(lambda: self.dispense_requested.emit(self.medication_data))
        self.dispense_button.setEnabled(quantity > 0)
        button_layout.addWidget(self.dispense_button)
        
        layout.addLayout(button_layout)
        
        # Style the widget
        self.setStyleSheet("""
            MedicationListItem {
                background-color: white;
                border: 1px solid #ddd;
                border-radius: 8px;
                margin: 2px;
            }
            MedicationListItem:hover {
                background-color: #f8f9fa;
                border-color: #007acc;
            }
        """)
        
    def update_medication_data(self, medication_data: dict):
        """Update the displayed medication data."""
        self.medication_data = medication_data
        self.name_label.setText(medication_data.get('name', 'Unknown'))
        quantity = medication_data.get('quantity', 0)
        self.quantity_label.setText(f"Quantity: {quantity} pills")
        self.dispense_button.setEnabled(quantity > 0)