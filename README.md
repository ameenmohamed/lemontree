# lemontree
# Sample Test Data
test<RT>remote temp12.12</RT><LT>localtemp10.10</LT><RL>remoteLux123</RL>,<WS>windSpeed</WS>,<RSM>remote soil moist</RSM>,<LSM>local soilMoist</LSM><RR> remote rain0</RR><LR>local riN</LR><RMST>1</RMST><LMST></LMST><TM>time in 24hrs14</TM><SS>sunset time in 245 hrs</SS>

test<RT>12.12</RT><LT>10.10</LT><RL>123</RL>,<WS>20</WS>,<RSM>110</RSM><LSM>112</LSM><RR>1</RR><LR>0</LR><RMST>1</RMST><LMST>0</LMST><TM>13</TM><SS>19</SS><SR>06</SR>- excess water

test<RT>12.12</RT><LT>10.10</LT><RL>123</RL>,<WS>400</WS>,<RSM>110</RSM><LSM>112</LSM><RR>1</RR><LR>0</LR><RMST>1</RMST><LMST>0</LMST><TM>13</TM><SS>19</SS><SR>06</SR> -- high winds

test<RT>12.12</RT><LT>10.10</LT><RL>123</RL>,<WS>20</WS>,<RSM>110</RSM><LSM>20</LSM><RR>1</RR><LR>0</LR><RMST>1</RMST><LMST>0</LMST><TM>13</TM><SS>19</SS><SR>06</SR> -- less water


LR : local rain not needed yet   <RMST>1</RMST><LMST>0</LMST>
RMST : remote mist , not needed


RT : remote temp
LT : local temp
RL : remote light
WS : Wind speed
RSM : remote soil Moist
LSM : Local soil moist
RR : is raining 0 no  , 1 yes
LMST : local mist
TM : time
SS : sunset
SR : sunrise


test<RT>12.12</RT><LT>10.10</LT><RL>123</RL><WS>20</WS>
test<RSM>110</RSM><LSM>20</LSM><RR>1</RR><LR>0</LR>
test<TM>13</TM><SS>07</SS><SR>18</SR>


-- less water

test<SS>07</SS><SR>18</SR><TM>19</TM>   - test time

test<RT>12.12</RT><LT>10.10</LT><RL>123</RL><WS>50</WS>  -- test HIGH winds heating
