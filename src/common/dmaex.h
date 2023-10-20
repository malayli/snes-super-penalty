/*! \fn  setModeHdmaWindow67(u8 bgrnd, u8* hdmatableL,u8* hdmatableR)
    \brief Do a window effect on screen. Use HDMA Channels 6 & 7.
    \param bgrnd background for the windows effect  (value MSWIN_BG1..MSWIN_BG4)
    \param bgrndmask background mask (inside, outside) for the windows effect  (value MSWIN1_BG13MSKIN..MSWIN1_BG13MSKIN)
    \param hdmatableL table with windows effect on the left
    \param hdmatableR table with windows effect on the right
*/    
void setModeHdmaWindow67(u8 bgrnd, u8 bgrndmask,u8* hdmatableL,u8* hdmatableR);
