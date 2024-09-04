
#ifndef RT_TREEDEF
#define RT_TREEDEF

// Option Definitions
// -- Settings
#define RT_INFOWINDOWHEIGHT					50						// Information window height
#define RT_DEFHEIGHT						21						// Default height of an item
#define RT_SPACE							5						// Default horizontal spacing
#define RT_EXPANDBOX						9						// Size of the expand box
#define RT_CHECKBOX							14						// Size of the check box
#define RT_EXPANDCOLUMN						16						// Width of the expand column
#define RT_PNINDENT							16						// Child level indent
#define RT_COLRNG							5						// Width of splitter
#define RT_EXPANDBOXHALF					(RT_EXPANDBOX / 2)		// Half of expand box
#define RT_DEFLABEL							_T("No Item Selected")	// Default label for no selected item in the info window
#define RT_DEFINFO							_T("Select an item to see its description.") // Default info for no selected item in the info window
#define RT_RESIZEBUFFER						25						// Resize limit for right and left of client so bar doesn't become invisible
#define RT_TREELIST_ID						30000					// ID of the list tree
#define RT_TIMER							1000					// The ID for a timer
// -- Item Settings
#define RT_ITEM_NONE						0
#define RT_ITEM_STATIC						1
#define RT_ITEM_CHECKBOX					2
#define RT_ITEM_COLOR						3
#define RT_ITEM_COMBOBOX					4
#define RT_ITEM_DATE						5
#define RT_ITEM_EDIT						6
#define RT_ITEM_IMAGE						7
#define RT_ITEM_RADIO						8
#define RT_ITEM_SPINNER						9
#define RT_ITEM_FONT						10
#define RT_ITEM_FILE						11
#define RT_ITEM_IPADDRESS					12
#define RT_ITEM_BUTTON					    13
// -- Tree Items
#define RT_OPTIONS_SHOWINFOWINDOW			0x0001					// Show information window
#define RT_OPTIONS_NOTIFY					0x0002					// Send parent notifications		
#define RT_OPTIONS_DEFINFOTEXTNOSEL			0x0004					// Show default info text for no selected item, otherwise blank		
#define RT_OPTIONS_SHADEEXPANDCOLUMN		0x0008					// Shade the expand column
#define RT_OPTIONS_SHADEROOTITEMS			0x0010					// Shade the root items
// -- Edit Items
#define RT_EDIT_MLHEIGHT					75						// Multiline height
// -- Combo Box Items
#define RT_COMBO_DROPDOWNHEIGHT				100						// Drop down default height
// -- Check Box Items
#define RT_CHECKBOX_DEFCHECKTEXT			_T("Checked")			// Default checked text
#define RT_CHECKBOX_DEFUNCHECKTEXT			_T("UnChecked")			// Default un checked text
#define RT_CHECKBOX_SIZE					14.2					// Size of the check box
// -- Radio Items
#define RT_RADIO_VSPACE						2						// Vertical space between radios
#define RT_RADIO_SIZE						14.2					// Size of radio
// -- Spinner Items
#define RT_SPINNER_WIDTH					15						// Width of the spinner button
// -- Color Items
#define RT_COLOR_MORECOLORS					_T("More Colors...")	// Text for more colors
#define RT_COLOR_AUTOMATIC					_T("Automatic")			// Text for more automatic
#define RT_COLOR_SIZE						14.2					// The size for the color square
// -- Image Items
#define RT_IMAGE_MARGIN						10						// The margin for the popup window
#define RT_IMAGE_IMAGESPACE					10						// The space between images in the popup window
#define RT_IMAGE_NOSELECTION				_T("No selection made.")// Text for no selection selected
#define RT_IMAGE_MAXIMAGES					100						// The maximum number of images
// -- File Items
#define RT_FILE_NOSELECTION					_T("No selection made.")// Text for no selection selected

// NOTE: The following are highly important and should not need to be changed
// --------------------------------------------------------------------------


// Definitions
// -- Hit test
#define RT_HIT_LABEL						(WM_USER + 1000)		// Label
#define RT_HIT_COLUMN						(WM_USER + 1001)		// Column
#define RT_HIT_EXPAND						(WM_USER + 1002)		// Expand
#define RT_HIT_ATTRIBUTE					(WM_USER + 1003)		// Attribute
#define RT_HIT_CLIENT						(WM_USER + 1004)		// Client
// -- Notification to user
#define RT_NOTIFY_FIRST						(0U-1100U)
#define RT_NOTIFY_INSERTITEM				(RT_NOTIFY_FIRST - 1)	// Insert item
#define RT_NOTIFY_DELETEITEM				(RT_NOTIFY_FIRST - 2)	// Delete item
#define RT_NOTIFY_DELETEALLITEMS			(RT_NOTIFY_FIRST - 3)	// Delete all items
#define RT_NOTIFY_ITEMCHANGED				(RT_NOTIFY_FIRST - 5)	// Item changed
#define RT_NOTIFY_ITEMBUTTONCLICK			(RT_NOTIFY_FIRST - 6)	// Item button click
#define RT_NOTIFY_SELCHANGE					(RT_NOTIFY_FIRST - 7)	// Selection changed
#define RT_NOTIFY_ITEMEXPANDING				(RT_NOTIFY_FIRST - 8)	// Item expanding
#define RT_NOTIFY_COLUMNCLICK				(RT_NOTIFY_FIRST - 9)	// Column click
#define RT_NOTIFY_PROPCLICK					(RT_NOTIFY_FIRST - 10)	// Property click
#define RT_NOTIFY_COLUMNRBUTTONUP			(RT_NOTIFY_FIRST - 11)	// Right button up
#define RT_NOTIFY_LBUTTONUP					(RT_NOTIFY_FIRST - 12)	// Left button up
#define RT_NOTIFY_VSCROLL					(RT_NOTIFY_FIRST - 13)	// Vertical scroll
#define RT_NOTIFY_HSCROLL					(RT_NOTIFY_FIRST - 14)	// Horizontal scroll
// -- Notication to controls
#define RT_NOTIFY_COMMITCHANGES				WM_USER + 0x0102		// Loosing focus
#define RT_NOTIFY_FORCEREDRAW				WM_USER + 0x0103		// Force redraw
#define RT_NOTIFY_UP						WM_USER + 0x0104		// Up key pressed
#define RT_NOTIFY_DOWN						WM_USER + 0x0105		// Down key pressed
// -- Menu Definitions
#define RT_MES_UNDO							_T("&Undo")				// Text for undo
#define RT_MES_CUT							_T("Cu&t")				// Text for cut
#define RT_MES_COPY							_T("&Copy")				// Text for copy
#define RT_MES_PASTE						_T("&Paste")			// Text for paste
#define RT_MES_DELETE						_T("&Delete")			// Text for delete
#define RT_MES_SELECTALL					_T("Select &All")		// Text for select all
#define RT_MES_NSELECTALL					WM_USER + 0x7000		// Command for select all
// -- Color  Items
#define RT_COLOR_SELCHANGE					WM_USER + 1001			// Color picker selection change
#define RT_COLOR_DROPDOWN					WM_USER + 1002			// Color picker drop down
#define RT_COLOR_CLOSEUP					WM_USER + 1003			// Color picker close up
#define RT_COLOR_SELENDOK					WM_USER + 1004			// Color picker end OK
#define RT_COLOR_SELENDCANCEL				WM_USER + 1005			// Color picker end (cancelled)
#define RT_COLOR_DEFAULTBOXVALUE			-3						// Default box value
#define RT_COLOR_CUSTOMBOXVALUE				-2						// Custom box value
#define RT_COLOR_INVALIDCOLOR				-1						// Invalid color value
#define RT_COLOR_MAXCOLORS					100						// Maximum number of colors
// -- Image  Items
#define RT_IMAGE_CLOSE						WM_USER + 1001			// Image picker window close
// -- Font Selection Items
#define RT_FS_NOTIFY_APPLY					WM_USER + 1000			// Apply notification
// -- IP Address
#define RT_IPADDRESS_KILLFOCUS				WM_USER + 1002			// Edit is loosing focus
#define RT_IPADDRESS_NEXTEDIT				WM_USER + 1003			// Edit needs to change focus to next edit
#define RT_IPADDRESS_NEXTITEM				WM_USER + 1004			// Next Item
#define RT_IPADDRESS_PREVITEM				WM_USER + 1005			// Prev Item

// Options
// -- Edit Items
#define RT_EDIT_MULTILINE					0x00040000L				// Multiline edit
#define RT_EDIT_PASSWORD					0x00000400L				// Password edit
#define RT_EDIT_NUMERICAL					0x00000200L				// Numerical edit
// -- Check Box Items
#define RT_CHECKBOX_SHOWTEXT				0x00040000L				// Show check text	
#define RT_CHECKBOX_SHOWCHECK				0x00000400L				// Show check box
// -- Spinner Items
#define RT_EDIT_WRAPAROUND					0x00040000L				// Wrap around
#define RT_EDIT_USEREDIT					0x00000400L				// Allow user edit
// -- Color Items
#define RT_COLOR_SHOWHEX					0x00040000L				// Show hex instead of RGB
#define RT_COLOR_LIVEUPDATE					0x00000400L				// Allow  smaple to show updates
// -- Image Items
#define RT_IMAGE_SHOWTEXT					0x00040000L				// Show text in item sample
// -- Font Selection Items
#define RT_FS_TTONLY						0x00040000L				// True type fonts only
#define RT_FS_USEDEFAULT					0x00000400L				// Default button
#define RT_FS_USEAPPLY						0x00000200L				// Apply button
#define RT_FS_USECHARFORMAT					0x00000100L				// Char format
#define RT_FS_CUSTOMSAMPLE					0x00000800L				// Custom sample
#define RT_FS_FONTNAMESAMPLE				0x00001000L				// Use font name sample
#define RT_FS_NOTEXTCOLOR					0x00020000L				// No text color
#define RT_FS_NOEFFECTS						0x00008000L				// No effects
#define RT_FS_NOSTYLES						0x00800000L				// No styles
#define RT_FS_NOFACE						0x00080000L				// No face
// -- File Items
#define RT_FILE_SHOWFULLPATH				0x00040000L				// Show full paths in sample
#define RT_FILE_SHOWFILENAME				0x00000400L				// Show file name in sample
#define RT_FILE_SHOWFILETITLE				0x00000200L				// Show file title in sample
#define RT_FILE_SHOWFILEEXT					0x00000100L				// Show file extention in sample
#define RT_FILE_SHOWFILEDIR					0x00000800L				// Show file directory in sample
#define RT_FILE_SHOWFILEDRIVE				0x00001000L				// Show file drive in sample
#define RT_FILE_OPENDIALOG					0x00020000L				// Open file dialog
#define RT_FILE_SELECTDIALOG				0x00008000L				// Select folder
// -- Hyperlink
#define RT_HL_HOVER							0x00040000L				// Use hover color
#define RT_HL_VISITED						0x00000400L				// Use visited color
#define RT_HL_UNDERLINEHOVER				0x00000200L				// Underline when mouse is hover
#define RT_HL_UNDERLINE						0x00000100L				// Underline always

#endif // !RT_TREEDEF

