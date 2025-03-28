#<checksum>150a144821ad3963ff6ccd1affb95b7b49ca2a2c0ede26af7b6b8b5517b5a93a</checksum>
#                                                                                
# Copyright (c) Infineon Technologies AG                                   
# All rights reserved.                                                           
#                                                                                
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:                  
#                                                                                
#  Redistributions of source code must retain the above copyright notice, this   
#  list of conditions and the following disclaimer.                              
#                                                                                
#  Redistributions in binary form must reproduce the above copyright notice,     
#  this list of conditions and the following disclaimer in the documentation     
#  and/or other materials provided with the distribution.                        
#                                                                                
#  Neither the name of the copyright holders nor the names of its contributors   
#  may be used to endorse or promote products derived from this software without 
#  specific prior written permission.                                            
#                                                                                
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED  
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE         
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE   
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL     
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR     
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER     
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,  
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.           
#
#
# \file     uart.py
#
# \brief    Script to handle uart elements in Config Wizard
#
# \version  V0.1.2
# \date     30. Nov 2021
#
###############################################################################
#                              Author(s) Identity                             #
#*****************************************************************************#
#  Initials     Name                                                          #
#  -------------------------------------------------------------------------- #
#  BG           Blandine Guillot                                              #
###############################################################################

###############################################################################
#                           Revision Control History                          #
#*****************************************************************************#
#  V0.1.0: 2020-11-25, BG:   Initial version                                  #
#  V0.1.1: 2021-06-11, BG:   EP-837: Added a connect to update the baudrate   #
#                            when the UART frequency is changed in SCU        #
#  V0.1.2: 2021-11-30, JO:   EP-937: Updated copyright and branding           #
###############################################################################

# -*- coding: utf-8 -*-
import math

def UARTx_updateMode(*args):
    if args[0].currentIndex == 0:
        xMSB = '20'
    else:
        xMSB = '0'
    return (args[0].currentIndex + 1, xMSB)

def UARTx_updateBit(*args):
    if args[0].currentIndex == 0:
        bit = 8
        bit9RX = '1'
        bit9TX = '1'
    elif (args[0].currentIndex == 1) or (args[0].currentIndex == 2):
        bit = 9
        bit9RX = '9th'
        bit9TX = '9th'
    return (bit, bit9RX, bit9TX)

def UARTx_updateBaudrateName(*args):
    if args[0].currentIndex == 1:
        baudrate_name = 'Fixed'
    elif (args[0].currentIndex == 0) or (args[0].currentIndex == 2):
        baudrate_name = 'Variable'
    return baudrate_name

def UARTx_updateBRParams(*args):
    # Calculation of the different parameters is done in Python
    # args[0], mode selected
    # args[1], baudrate value selected manually or automatically (spinbox)
    # args[2], SCU_fUART_MHz
    fUART_MHz = int(args[2].text.replace(args[2].suffix, ''))
    deviation = 0
    min_deviation = 0.002
    flag_sign = 0
    index_flag_sign = 1
    flag = 0
    if (args[0].currentIndex == 0) or (args[0].currentIndex == 2):
        max_BR = fUART_MHz * 1000000 / 16
        BR_prescaler = 5
        BR_prescalerVal = 32
        BR_reloadVal = 0
        if args[1] <= max_BR:
            while ((BR_prescaler >= 0) and (deviation <= 0.002)):
                # According to UM, Baudrate = fUART / (16 * prescaler * (reloadVal + fracDiv/32))
                BR_reloadVal = math.floor((fUART_MHz * 1000000) / (16 * BR_prescalerVal * args[1]))
                # Check if it is a 11-bit value (0-2047)
                if BR_reloadVal <= 2047:
                    # The possible fractional divider is between u8_minFracDiv and u8_maxFracDiv
                    for i in range(1,33):
                        calc_BR = float('%.5f'%((fUART_MHz * 1000000) / (16 * BR_prescalerVal * (BR_reloadVal + i/32))))
                        min_BR = float('%.5f'%(args[1] * (1 - deviation)))
                        max_BR = float('%.5f'%(args[1] * (1 + deviation)))
                        if (calc_BR >= min_BR) and (calc_BR <= max_BR):
                            flag = 1
                            prescaler_used = BR_prescaler
                            prescalerVal_used = BR_prescalerVal
                            if i == 32:
                                fracDiv_used = 0
                                reloadVal_used = BR_reloadVal + 1
                            else:
                                fracDiv_used = i
                                reloadVal_used = BR_reloadVal
                            break
                    if flag == 0:
                        # No fractional divider was found, the prescaler value is divided by 2 and decremented
                        BR_prescaler -= 1
                        BR_prescalerVal /= 2
                        if BR_prescaler == -1:
                            if flag_sign == 0:
                                deviation = float('%.4f'%(deviation + index_flag_sign * 0.0001))
                                flag_sign = 1
                            elif flag_sign == 1:
                                deviation = float('%.4f'%(deviation - index_flag_sign * 0.0001))
                                flag_sign = 0
                            index_flag_sign += 1
                            # Reinitialization of variables
                            BR_prescaler = 5
                            BR_prescalerVal = 32
                            BR_reloadVal = 0
                    elif flag == 1:
                        # Getting the minimum deviation
                        if abs(deviation) < min_deviation:
                            #print('Better deviation ' + str(abs(deviation)) + ', ' + str(min_deviation))
                            min_deviation = abs(deviation)
                            prescaler_used = BR_prescaler
                            prescalerVal_used = BR_prescalerVal
                            fracDiv_used = i
                            reloadVal_used = BR_reloadVal
                        #print('BR_prescaler ' + str(BR_prescaler) + ', BR_reloadVal ' + str(BR_reloadVal) + ', BR_fracDiv ' + str(i) + ', deviation ' + str(deviation))
                        BR_prescaler -= 1
                        BR_prescalerVal /= 2
                else:
                    BR_prescaler -= 1
                    BR_prescalerVal /= 2
        BR_effective = str(math.floor((fUART_MHz * 1000000) / (16 * prescalerVal_used * (reloadVal_used + fracDiv_used/32))))
        BR_deviation = str(min_deviation*100)[0:4]
    elif args[0].currentIndex == 1:
        (prescalerVal_used, fracDiv_used, reloadVal_used) = (64, 0, 1)
        BR_effective = str(math.floor(fUART_MHz * 1000000 / 64))
        BR_deviation = '0'
    return (round(prescalerVal_used), fracDiv_used, reloadVal_used, BR_effective, BR_deviation)

def UARTx_updateBaudrate(*args):
    # args[0], mode for the baudrate generation, 0: automatic and 1:manual $IDX$.BR_CONFIG_EDIT
    # args[1], baudrate value selected manually (spinbox)
    # args[2], baudrate value selected automatically (spinbox)
    # args[3], mode selected
    # args[4], SCU_fUART_MHz
    BR_prescalerVal = 0
    BR_fracDiv = 0
    BR_reloadVal = 0
    BR_effective = 0
    BR_deviation = 0
    if args[0].text == '0':
        (BR_prescalerVal, BR_fracDiv, BR_reloadVal, BR_effective, BR_deviation) = UARTx_updateBRParams(args[3], int(args[2].currentData), args[4])
    elif args[0].text == '1':
        (BR_prescalerVal, BR_fracDiv, BR_reloadVal, BR_effective, BR_deviation) = UARTx_updateBRParams(args[3], args[1].value, args[4])
    return (BR_prescalerVal, BR_fracDiv, BR_reloadVal, BR_effective, BR_deviation)

def UARTx_updateSerialReception(*args):
    # args[0], SCON_REN
    if int(args[0].checked) == 0:
        enrx = 'Disabled'
    elif int(args[0].checked) == 1:
        enrx = 'Enabled'
    return enrx

def UARTx_showMode(*args):
    # args[0], UART0 or UART1
    # args[1], mode selected
    # args[2], mode for the baudrate generation, 0: automatic and 1:manual $IDX$.BR_CONFIG_EDIT
    # args[3], RIINTEN checkbox
    # args[4], TIINTEN checkbox
    # args[5], Transmit Start Trigger Selection
    # args[6], Enable Break/Sync Detection
    # args[7], End of Sync Interrupt _EOS_INT_EN
    # args[8], Sync Error Interrupt _SYNCERR_INT_EN
    # General settings
    application.runScript('setDef(' + args[0] + '_' + args[0] + '.INSEL_RXDSEL.visible=1)')
    application.runScript('setDef(UART1_UART0.INSEL_RXDSEL.visible=0)')
    application.runScript('setDef(UART0_UART1.INSEL_RXDSEL.visible=0)')
    application.runScript('setDef(' + args[0] + '_' + args[0] + '.INSEL_TXEVSEL.visible=1)')
    application.runScript('setDef(UART1_UART0.INSEL_TXEVSEL.visible=0)')
    application.runScript('setDef(UART0_UART1.INSEL_TXEVSEL.visible=0)')
    application.runScript('setDef(' + args[0] + '_' + args[0] + '.OUTPUT_TXDSEL.visible=1)')
    application.runScript('setDef(UART1_UART0.OUTPUT_TXDSEL.visible=0)')
    application.runScript('setDef(UART0_UART1.OUTPUT_TXDSEL.visible=0)')
    if int(args[3].checked) == 0:
        application.runScript('setDef(CONT_' + args[0] + '_RINODE.enabled=0)')
        application.runScript('setDef(CONT_' + args[0] + '_RIEN.enabled=0)')
        application.runScript('setDef(CONT_' + args[0] + '_RICB.enabled=0)')
    elif int(args[3].checked) == 1:
        application.runScript('setDef(CONT_' + args[0] + '_RINODE.enabled=1)')
        application.runScript('setDef(CONT_' + args[0] + '_RIEN.enabled=1)')
        application.runScript('setDef(CONT_' + args[0] + '_RICB.enabled=1)')
    if int(args[4].checked) == 0:
        application.runScript('setDef(CONT_' + args[0] + '_TINODE.enabled=0)')
        application.runScript('setDef(CONT_' + args[0] + '_TIEN.enabled=0)')
        application.runScript('setDef(CONT_' + args[0] + '_TICB.enabled=0)')
    elif int(args[4].checked) == 1:
        application.runScript('setDef(CONT_' + args[0] + '_TINODE.enabled=1)')
        application.runScript('setDef(CONT_' + args[0] + '_TIEN.enabled=1)')
        application.runScript('setDef(CONT_' + args[0] + '_TICB.enabled=1)')
    if args[5].currentIndex == 2:
        application.runScript('setDef(CONT_' + args[0] + '_TXSTARTINP.visible=1)')
    else:
        application.runScript('setDef(CONT_' + args[0] + '_TXSTARTINP.visible=0)')
    if int(args[6].checked) == 0:
        application.runScript('setDef(CONT_' + args[0] + '_ENEOS.enabled=0)')
        application.runScript('setDef(CONT_' + args[0] + '_EOSNODE.enabled=0)')
        application.runScript('setDef(CONT_' + args[0] + '_EOSEN.enabled=0)')
        application.runScript('setDef(CONT_' + args[0] + '_EOSCB.enabled=0)')
        application.runScript('setDef(CONT_' + args[0] + '_ENSYNCERR.enabled=0)')
        application.runScript('setDef(CONT_' + args[0] + '_SYNCERRNODE.enabled=0)')
        application.runScript('setDef(CONT_' + args[0] + '_SYNCERREN.enabled=0)')
        application.runScript('setDef(CONT_' + args[0] + '_SYNCERRCB.enabled=0)')
    elif int(args[6].checked) == 1:
        application.runScript('setDef(CONT_' + args[0] + '_ENEOS.enabled=1)')
        application.runScript('setDef(CONT_' + args[0] + '_ENSYNCERR.enabled=1)')
        if int(args[7].checked) == 0:
            application.runScript('setDef(CONT_' + args[0] + '_EOSNODE.enabled=0)')
            application.runScript('setDef(CONT_' + args[0] + '_EOSEN.enabled=0)')
            application.runScript('setDef(CONT_' + args[0] + '_EOSCB.enabled=0)')
        elif int(args[7].checked) == 1:
            application.runScript('setDef(CONT_' + args[0] + '_EOSNODE.enabled=1)')
            application.runScript('setDef(CONT_' + args[0] + '_EOSEN.enabled=1)')
            application.runScript('setDef(CONT_' + args[0] + '_EOSCB.enabled=1)')
        if int(args[8].checked) == 0:
            application.runScript('setDef(CONT_' + args[0] + '_SYNCERRNODE.enabled=0)')
            application.runScript('setDef(CONT_' + args[0] + '_SYNCERREN.enabled=0)')
            application.runScript('setDef(CONT_' + args[0] + '_SYNCERRCB.enabled=0)')
        elif int(args[8].checked) == 1:
            application.runScript('setDef(CONT_' + args[0] + '_SYNCERRNODE.enabled=1)')
            application.runScript('setDef(CONT_' + args[0] + '_SYNCERREN.enabled=1)')
            application.runScript('setDef(CONT_' + args[0] + '_SYNCERRCB.enabled=1)')
    # Mode specific
    if (args[1].currentIndex == 0) or (args[1].currentIndex == 2):
        application.runScript('setDef(CONT_' + args[0] + '_BR_SETTINGS.enabled=1)')
        if args[2].text == '0': # Automatic BR generation
            application.runScript('setDef(CONT_' + args[0] + '_BR_AUTO.enabled=1)')
            application.runScript('setDef(CONT_' + args[0] + '_BR_MAN.enabled=0)')
        elif args[2].text == '1': # Manual BR generation
            application.runScript('setDef(CONT_' + args[0] + '_BR_AUTO.enabled=0)')
            application.runScript('setDef(CONT_' + args[0] + '_BR_MAN.enabled=1)')
    elif args[1].currentIndex == 1:
        application.runScript('setDef(CONT_' + args[0] + '_BR_SETTINGS.enabled=0)')
        
def UARTx_updateColors(*args):
    # args[0], UART0 or UART1
    # args[1], selected mode
    # args[2], IEN_RIEN
    # args[3], LINCON_BREN
    # args[4], SCON_TXINV
    # args[5], IEN_TIEN
    # args[6], SCON_TXENSEL
    # args[7], SCON_REN
    application.runScript('setDef(' + args[0] + '_1=#000000)')
    application.runScript('setDef(' + args[0] + '_2=#F0F0F0)')
    application.runScript('setDef(' + args[0] + '_18=#0000FF)')
    application.runScript('setDef(' + args[0] + '_21=#FF0000)')
    if (args[1].currentIndex == 0) or (args[1].currentIndex == 2):
        application.runScript('setDef(' + args[0] + '_3=#FFFFF0)')
        application.runScript('setDef(' + args[0] + '_4=#000000)')
        application.runScript('setDef(' + args[0] + '_5=#F0F0F0)')
    elif args[1].currentIndex == 1:
        application.runScript('setDef(' + args[0] + '_3=#000000)')
        application.runScript('setDef(' + args[0] + '_4=#FFFFF0)')
        application.runScript('setDef(' + args[0] + '_5=#FFFFF0)')
    if int(args[2].checked) == 0:
        application.runScript('setDef(' + args[0] + '_6=#FFFFF0)')
    elif int(args[2].checked) == 1:
        application.runScript('setDef(' + args[0] + '_6=#000000)')
    if int(args[3].checked) == 0:
        application.runScript('setDef(' + args[0] + '_12=#FFFFF0)')
        application.runScript('setDef(' + args[0] + '_13=#FFFFF0)')
    elif int(args[3].checked) == 1:
        application.runScript('setDef(' + args[0] + '_12=#000000)')
        application.runScript('setDef(' + args[0] + '_13=#F0F0F0)')
    if int(args[4].checked) == 0:
        application.runScript('setDef(' + args[0] + '_14=#000000)')
        application.runScript('setDef(' + args[0] + '_15=#FFFFF0)')
    elif int(args[4].checked) == 1:
        application.runScript('setDef(' + args[0] + '_14=#FFFFF0)')
        application.runScript('setDef(' + args[0] + '_15=#000000)')
    if int(args[5].checked) == 0:
        application.runScript('setDef(' + args[0] + '_16=#FFFFF0)')
    elif int(args[5].checked) == 1:
        application.runScript('setDef(' + args[0] + '_16=#000000)')
    if args[6].currentIndex == 2:
        application.runScript('setDef(' + args[0] + '_17=#000000)')
    else:
        application.runScript('setDef(' + args[0] + '_17=#F0F0F0)')
    if int(args[7].checked) == 0:
        application.runScript('setDef(' + args[0] + '_7=#F0F0F0)')
        application.runScript('setDef(' + args[0] + '_8=#F0F0F0)')
        application.runScript('setDef(' + args[0] + '_10=#F0F0F0)')
        application.runScript('setDef(' + args[0] + '_11=#F0F0F0)')
        application.runScript('setDef(' + args[0] + '_9=#F0F0F0)')
        application.runScript('setDef(' + args[0] + '_22=#F0F0F0)')
        if (args[1].currentIndex == 1) or (args[1].currentIndex == 2):
            application.runScript('setDef(' + args[0] + '_19=#F0F0F0)')
            application.runScript('setDef(' + args[0] + '_20=#0000FF)')
        elif args[1].currentIndex == 0:
            application.runScript('setDef(' + args[0] + '_19=#0000FF)')
            application.runScript('setDef(' + args[0] + '_20=#F0F0F0)')
    elif int(args[7].checked) == 1:
        application.runScript('setDef(' + args[0] + '_7=#000000)')
        application.runScript('setDef(' + args[0] + '_8=#000000)')
        application.runScript('setDef(' + args[0] + '_9=#0000FF)')
        application.runScript('setDef(' + args[0] + '_10=#0000FF)')
        application.runScript('setDef(' + args[0] + '_11=#FF0000)')
        if (args[1].currentIndex == 1) or (args[1].currentIndex == 2):
            application.runScript('setDef(' + args[0] + '_19=#F0F0F0)')
            application.runScript('setDef(' + args[0] + '_20=#0000FF)')
            application.runScript('setDef(' + args[0] + '_23=#0000FF)')
            application.runScript('setDef(' + args[0] + '_22=#F0F0F0)')
        elif args[1].currentIndex == 0:
            application.runScript('setDef(' + args[0] + '_19=#0000FF)')
            application.runScript('setDef(' + args[0] + '_20=#F0F0F0)')
            application.runScript('setDef(' + args[0] + '_23=#F0F0F0)')
            application.runScript('setDef(' + args[0] + '_22=#0000FF)')

def UARTx_updateVisibility(*args):
    # args[0], UART0 or UART1
    # args[1], selected mode
    # args[2], SCON_TXINV
    # args[3], SCON_REN
    if args[1].currentIndex == 0:
        application.runScript('setDef(VIS_' + args[0] + '_1=hidden)')
        application.runScript('setDef(VIS_' + args[0] + '_2=visible)')
        application.runScript('setDef(VIS_' + args[0] + '_4=visible)')
        application.runScript('setDef(VIS_' + args[0] + '_5=hidden)')
    elif args[1].currentIndex == 1:
        application.runScript('setDef(VIS_' + args[0] + '_1=visible)')
        application.runScript('setDef(VIS_' + args[0] + '_2=hidden)')
        application.runScript('setDef(VIS_' + args[0] + '_4=hidden)')
        application.runScript('setDef(VIS_' + args[0] + '_5=visible)')
    elif args[1].currentIndex == 2:
        application.runScript('setDef(VIS_' + args[0] + '_1=hidden)')
        application.runScript('setDef(VIS_' + args[0] + '_2=visible)')
        application.runScript('setDef(VIS_' + args[0] + '_4=hidden)')
        application.runScript('setDef(VIS_' + args[0] + '_5=visible)')
    if int(args[2].checked) == 0:
        application.runScript('setDef(VIS_' + args[0] + '_3=hidden)')
    elif int(args[2].checked) == 1:
        application.runScript('setDef(VIS_' + args[0] + '_3=visible)')
    if int(args[3].checked) == 0:
        application.runScript('setDef(VIS_' + args[0] + '_6=hidden)')
        application.runScript('setDef(VIS_' + args[0] + '_7=hidden)')
    elif int(args[3].checked) == 1:
        if args[1].currentIndex == 0:
            application.runScript('setDef(VIS_' + args[0] + '_6=visible)')
            application.runScript('setDef(VIS_' + args[0] + '_7=hidden)')
        else:
            application.runScript('setDef(VIS_' + args[0] + '_6=hidden)')
            application.runScript('setDef(VIS_' + args[0] + '_7=visible)')

    
def updateModeUART0_connect(idx):
    (UART0MODE.text, UART0_xMSB.text) = UARTx_updateMode(UART0_MODE)
    (UART0BIT.text, UART0BIT9RX.text, UART0BIT9TX.text) = UARTx_updateBit(UART0_MODE)
    UART0BR.text = UARTx_updateBaudrateName(UART0_MODE)
    (UART0BRPRE.text, UART0FRACDIV.text, UART0RELVAL.text, UART0EFFBR.text, UART0_DEVIATION.text) = UARTx_updateBaudrate(UART0_BR_CONFIG_EDIT, UART0_BR_VAL_MAN, UART0_BR_VAL_AUTO, UART0_MODE, SCU_fUART_MHz)
    UART0_EFFBR.text = UART0EFFBR.text
    UART0RXINP.text = UART0_UART0_INSEL_RXDSEL.currentText
    UART0ENRX.text = UARTx_updateSerialReception(UART0_SCON_REN)
    UART0TXOUTPUT.text = UART0_UART0_OUTPUT_TXDSEL.currentText
    UART0STARTTRIG.text = UART0_SCON_TXENSEL.currentText
    UART0TXSTARTINP.text = UART0_UART0_INSEL_TXEVSEL.currentText
    UARTx_showMode('UART0', UART0_MODE, UART0_BR_CONFIG_EDIT, UART0_RI_INT_EN, UART0_TI_INT_EN, UART0_SCON_TXENSEL, UART0_LINCON_BREN, UART0_EOS_INT_EN, UART0_SYNCERR_INT_EN)
    UARTx_updateColors('UART0', UART0_MODE, UART0_IEN_RIEN, UART0_LINCON_BREN, UART0_SCON_TXINV, UART0_IEN_TIEN, UART0_SCON_TXENSEL, UART0_SCON_REN)
    UARTx_updateVisibility('UART0', UART0_MODE, UART0_SCON_TXINV, UART0_SCON_REN)
    application.runScript('setDef(UART0_Block.update=1)')

def updateModeUART1_connect(idx):
    (UART1MODE.text, UART1_xMSB.text) = UARTx_updateMode(UART1_MODE)
    (UART1BIT.text, UART1BIT9RX.text, UART1BIT9TX.text) = UARTx_updateBit(UART1_MODE)
    UART1BR.text = UARTx_updateBaudrateName(UART1_MODE)
    (UART1BRPRE.text, UART1FRACDIV.text, UART1RELVAL.text, UART1EFFBR.text, UART1_DEVIATION.text) = UARTx_updateBaudrate(UART1_BR_CONFIG_EDIT, UART1_BR_VAL_MAN, UART1_BR_VAL_AUTO, UART1_MODE, SCU_fUART_MHz)
    UART1_EFFBR.text = UART1EFFBR.text
    UART1RXINP.text = UART1_UART1_INSEL_RXDSEL.currentText
    UART1ENRX.text = UARTx_updateSerialReception(UART1_SCON_REN)
    UART1TXOUTPUT.text = UART1_UART1_OUTPUT_TXDSEL.currentText
    UART1STARTTRIG.text = UART1_SCON_TXENSEL.currentText
    UART1TXSTARTINP.text = UART1_UART1_INSEL_TXEVSEL.currentText
    UARTx_showMode('UART1', UART1_MODE, UART1_BR_CONFIG_EDIT, UART1_RI_INT_EN, UART1_TI_INT_EN, UART1_SCON_TXENSEL, UART1_LINCON_BREN, UART1_EOS_INT_EN, UART1_SYNCERR_INT_EN)
    UARTx_updateColors('UART1', UART1_MODE, UART1_IEN_RIEN, UART1_LINCON_BREN, UART1_SCON_TXINV, UART1_IEN_TIEN, UART1_SCON_TXENSEL, UART1_SCON_REN)
    UARTx_updateVisibility('UART1', UART1_MODE, UART1_SCON_TXINV, UART1_SCON_REN)
    application.runScript('setDef(UART1_Block.update=1)')

def updateBaudrateUART0_connect(idx):
    UART0BR.text = UARTx_updateBaudrateName(UART0_MODE)
    (UART0BRPRE.text, UART0FRACDIV.text, UART0RELVAL.text, UART0EFFBR.text, UART0_DEVIATION.text) = UARTx_updateBaudrate(UART0_BR_CONFIG_EDIT, UART0_BR_VAL_MAN, UART0_BR_VAL_AUTO, UART0_MODE, SCU_fUART_MHz)
    UART0_EFFBR.text = UART0EFFBR.text
    UARTx_showMode('UART0', UART0_MODE, UART0_BR_CONFIG_EDIT, UART0_RI_INT_EN, UART0_TI_INT_EN, UART0_SCON_TXENSEL, UART0_LINCON_BREN, UART0_EOS_INT_EN, UART0_SYNCERR_INT_EN)
    UARTx_updateColors('UART0', UART0_MODE, UART0_IEN_RIEN, UART0_LINCON_BREN, UART0_SCON_TXINV, UART0_IEN_TIEN, UART0_SCON_TXENSEL, UART0_SCON_REN)
    UARTx_updateVisibility('UART0', UART0_MODE, UART0_SCON_TXINV, UART0_SCON_REN)
    application.runScript('setDef(UART0_Block.update=1)')
    
def updateBaudrateUART1_connect(idx):
    UART1BR.text = UARTx_updateBaudrateName(UART1_MODE)
    (UART1BRPRE.text, UART1FRACDIV.text, UART1RELVAL.text, UART1EFFBR.text, UART1_DEVIATION.text) = UARTx_updateBaudrate(UART1_BR_CONFIG_EDIT, UART1_BR_VAL_MAN, UART1_BR_VAL_AUTO, UART1_MODE, SCU_fUART_MHz)
    UART1_EFFBR.text = UART1EFFBR.text
    UARTx_showMode('UART1', UART1_MODE, UART1_BR_CONFIG_EDIT, UART1_RI_INT_EN, UART1_TI_INT_EN, UART1_SCON_TXENSEL, UART1_LINCON_BREN, UART1_EOS_INT_EN, UART1_SYNCERR_INT_EN)
    UARTx_updateColors('UART1', UART1_MODE, UART1_IEN_RIEN, UART1_LINCON_BREN, UART1_SCON_TXINV, UART1_IEN_TIEN, UART1_SCON_TXENSEL, UART1_SCON_REN)
    UARTx_updateVisibility('UART1', UART1_MODE, UART1_SCON_TXINV, UART1_SCON_REN)
    application.runScript('setDef(UART1_Block.update=1)')

def updateReceptionSettingsUART0_connect(idx):
    UART0RXINP.text = UART0_UART0_INSEL_RXDSEL.currentText
    UART0ENRX.text = UARTx_updateSerialReception(UART0_SCON_REN)
    UARTx_updateColors('UART0', UART0_MODE, UART0_IEN_RIEN, UART0_LINCON_BREN, UART0_SCON_TXINV, UART0_IEN_TIEN, UART0_SCON_TXENSEL, UART0_SCON_REN)
    UARTx_updateVisibility('UART0', UART0_MODE, UART0_SCON_TXINV, UART0_SCON_REN)
    application.runScript('setDef(UART0_Block.update=1)')

def updateTransmitionSettingsUART0_connect(idx):
    UART0TXOUTPUT.text = UART0_UART0_OUTPUT_TXDSEL.currentText
    UART0STARTTRIG.text = UART0_SCON_TXENSEL.currentText
    UART0TXSTARTINP.text = UART0_UART0_INSEL_TXEVSEL.currentText
    UARTx_showMode('UART0', UART0_MODE, UART0_BR_CONFIG_EDIT, UART0_RI_INT_EN, UART0_TI_INT_EN, UART0_SCON_TXENSEL, UART0_LINCON_BREN, UART0_EOS_INT_EN, UART0_SYNCERR_INT_EN)
    UARTx_updateColors('UART0', UART0_MODE, UART0_IEN_RIEN, UART0_LINCON_BREN, UART0_SCON_TXINV, UART0_IEN_TIEN, UART0_SCON_TXENSEL, UART0_SCON_REN)
    UARTx_updateVisibility('UART0', UART0_MODE, UART0_SCON_TXINV, UART0_SCON_REN)
    application.runScript('setDef(UART0_Block.update=1)')

def updateReceptionSettingsUART1_connect(idx):
    UART1RXINP.text = UART1_UART1_INSEL_RXDSEL.currentText
    UART1ENRX.text = UARTx_updateSerialReception(UART1_SCON_REN)
    UARTx_updateColors('UART1', UART1_MODE, UART1_IEN_RIEN, UART1_LINCON_BREN, UART1_SCON_TXINV, UART1_IEN_TIEN, UART1_SCON_TXENSEL, UART1_SCON_REN)
    UARTx_updateVisibility('UART1', UART1_MODE, UART1_SCON_TXINV, UART1_SCON_REN)
    application.runScript('setDef(UART1_Block.update=1)')

def updateTransmitionSettingsUART1_connect(idx):
    UART1TXOUTPUT.text = UART1_UART1_OUTPUT_TXDSEL.currentText
    UART1STARTTRIG.text = UART1_SCON_TXENSEL.currentText
    UART1TXSTARTINP.text = UART1_UART1_INSEL_TXEVSEL.currentText
    UARTx_showMode('UART1', UART1_MODE, UART1_BR_CONFIG_EDIT, UART1_RI_INT_EN, UART1_TI_INT_EN, UART1_SCON_TXENSEL, UART1_LINCON_BREN, UART1_EOS_INT_EN, UART1_SYNCERR_INT_EN)
    UARTx_updateColors('UART1', UART1_MODE, UART1_IEN_RIEN, UART1_LINCON_BREN, UART1_SCON_TXINV, UART1_IEN_TIEN, UART1_SCON_TXENSEL, UART1_SCON_REN)
    UARTx_updateVisibility('UART1', UART1_MODE, UART1_SCON_TXINV, UART1_SCON_REN)
    application.runScript('setDef(UART1_Block.update=1)')

def updateInterruptsUART0_connect(idx):
    UARTx_showMode('UART0', UART0_MODE, UART0_BR_CONFIG_EDIT, UART0_RI_INT_EN, UART0_TI_INT_EN, UART0_SCON_TXENSEL, UART0_LINCON_BREN, UART0_EOS_INT_EN, UART0_SYNCERR_INT_EN)
    UARTx_updateColors('UART0', UART0_MODE, UART0_IEN_RIEN, UART0_LINCON_BREN, UART0_SCON_TXINV, UART0_IEN_TIEN, UART0_SCON_TXENSEL, UART0_SCON_REN)
    UARTx_updateVisibility('UART0', UART0_MODE, UART0_SCON_TXINV, UART0_SCON_REN)
    application.runScript('setDef(UART0_Block.update=1)')

def updateInterruptsUART1_connect(idx):
    UARTx_showMode('UART1', UART1_MODE, UART1_BR_CONFIG_EDIT, UART1_RI_INT_EN, UART1_TI_INT_EN, UART1_SCON_TXENSEL, UART1_LINCON_BREN, UART1_EOS_INT_EN, UART1_SYNCERR_INT_EN)
    UARTx_updateColors('UART1', UART1_MODE, UART1_IEN_RIEN, UART1_LINCON_BREN, UART1_SCON_TXINV, UART1_IEN_TIEN, UART1_SCON_TXENSEL, UART1_SCON_REN)
    UARTx_updateVisibility('UART1', UART1_MODE, UART1_SCON_TXINV, UART1_SCON_REN)
    application.runScript('setDef(UART1_Block.update=1)')
    
    
def UARTx_updateConfig_connect(state):
    updateModeUART0_connect(0)
    updateModeUART1_connect(0)
    
UART0_MODE.connect('currentIndexChanged(int)', updateModeUART0_connect)
UART1_MODE.connect('currentIndexChanged(int)', updateModeUART1_connect)

# Module enable
UART0_GLOBCONF_EN.connect('stateChanged(int)', UARTx_updateConfig_connect)
UART1_GLOBCONF_EN.connect('stateChanged(int)', UARTx_updateConfig_connect)

# Frequency
UART_FREQ_EDIT.connect('textChanged(QString)', updateBaudrateUART0_connect)
UART_FREQ_EDIT.connect('textChanged(QString)', updateBaudrateUART1_connect)

# Baudrate Generation
UART0_BR_VAL_AUTO.connect('currentIndexChanged(int)', updateBaudrateUART0_connect)
UART0_BR_VAL_MAN.connect('valueChanged(int)', updateBaudrateUART0_connect)
UART0_BR_CONFIG_EDIT.connect('textChanged(QString)', updateBaudrateUART0_connect)
UART1_BR_VAL_AUTO.connect('currentIndexChanged(int)', updateBaudrateUART1_connect)
UART1_BR_VAL_MAN.connect('valueChanged(int)', updateBaudrateUART1_connect)
UART1_BR_CONFIG_EDIT.connect('textChanged(QString)', updateBaudrateUART1_connect)

# Reception Settings
UART0_UART0_INSEL_RXDSEL.connect('currentIndexChanged(int)', updateReceptionSettingsUART0_connect)
UART0_SCON_REN.connect('stateChanged(int)', updateReceptionSettingsUART0_connect)

UART1_UART1_INSEL_RXDSEL.connect('currentIndexChanged(int)', updateReceptionSettingsUART1_connect)
UART1_SCON_REN.connect('stateChanged(int)', updateReceptionSettingsUART1_connect)

# Transmission Settings
UART0_UART0_OUTPUT_TXDSEL.connect('currentIndexChanged(int)', updateTransmitionSettingsUART0_connect)
UART0_SCON_TXENSEL.connect('currentIndexChanged(int)', updateTransmitionSettingsUART0_connect)
UART0_UART0_INSEL_TXEVSEL.connect('currentIndexChanged(int)', updateTransmitionSettingsUART0_connect)
UART0_SCON_TXINV.connect('stateChanged(int)', updateTransmitionSettingsUART0_connect)
UART0_LINCON_BREN.connect('stateChanged(int)', updateTransmitionSettingsUART0_connect)

UART1_UART1_OUTPUT_TXDSEL.connect('currentIndexChanged(int)', updateTransmitionSettingsUART1_connect)
UART1_SCON_TXENSEL.connect('currentIndexChanged(int)', updateTransmitionSettingsUART1_connect)
UART1_UART1_INSEL_TXEVSEL.connect('currentIndexChanged(int)', updateTransmitionSettingsUART1_connect)
UART1_SCON_TXINV.connect('stateChanged(int)', updateTransmitionSettingsUART1_connect)
UART1_LINCON_BREN.connect('stateChanged(int)', updateTransmitionSettingsUART1_connect)

# Interrupts
UART0_IEN_RIEN.connect('stateChanged(int)', updateInterruptsUART0_connect)
UART0_RI_INT_EN.connect('stateChanged(int)', updateInterruptsUART0_connect)
UART0_IEN_TIEN.connect('stateChanged(int)', updateInterruptsUART0_connect)
UART0_TI_INT_EN.connect('stateChanged(int)', updateInterruptsUART0_connect)
UART0_IEN_EOFSYNEN.connect('stateChanged(int)', updateInterruptsUART0_connect)
UART0_EOS_INT_EN.connect('stateChanged(int)', updateInterruptsUART0_connect)
UART0_IEN_ERRSYNEN.connect('stateChanged(int)', updateInterruptsUART0_connect)
UART0_SYNCERR_INT_EN.connect('stateChanged(int)', updateInterruptsUART0_connect)

UART1_IEN_RIEN.connect('stateChanged(int)', updateInterruptsUART1_connect)
UART1_RI_INT_EN.connect('stateChanged(int)', updateInterruptsUART1_connect)
UART1_IEN_TIEN.connect('stateChanged(int)', updateInterruptsUART1_connect)
UART1_TI_INT_EN.connect('stateChanged(int)', updateInterruptsUART1_connect)
UART1_IEN_EOFSYNEN.connect('stateChanged(int)', updateInterruptsUART1_connect)
UART1_EOS_INT_EN.connect('stateChanged(int)', updateInterruptsUART1_connect)
UART1_IEN_ERRSYNEN.connect('stateChanged(int)', updateInterruptsUART1_connect)
UART1_SYNCERR_INT_EN.connect('stateChanged(int)', updateInterruptsUART1_connect)

# Executing init functions
updateModeUART0_connect(0)
updateModeUART1_connect(0)