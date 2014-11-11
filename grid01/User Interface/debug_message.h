#ifndef __grid_debug_message__
#define __grid_debug_message__

#define debug_message_line "\t------------------------------------------------------------"
#define debug_message_line_fatal "\t************************************************************"

#ifdef __debug__

#define debug_message_called(name) cui << debug_message_line << "\n\t\t" << (name) << " called.\n" << debug_message_line << "\n\n"
#define debug_message_returned(name) cui << debug_message_line << "\n\t\t" << "returned to " << (name) << "\n" << debug_message_line << "\n\n"

#else
#define debug_message_called(name) NULL
#define debug_message_returned(name) NULL

#endif

#endif
