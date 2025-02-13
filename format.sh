echo -e "--- start encode conv ---"
python ./Scripts/Format/EncodeConv.py Source/Example
python ./Scripts/Format/EncodeConv.py Source/MechanicEngine
python ./Scripts/Format/EncodeConv.py Source/EmptyApp
python ./Scripts/Format/EncodeConv.py Source/Sandbox
python ./Scripts/Format/EncodeConv.py Source/MechanicPic
echo -e "--- finish encode conv ---"

echo -e "\n"

echo -e "--- start clang-format ---"
python ./Scripts/Format/FormatFile.py Source/Example
python ./Scripts/Format/FormatFile.py Source/MechanicEngine
python ./Scripts/Format/FormatFile.py Source/EmptyApp
python ./Scripts/Format/FormatFile.py Source/Sandbox
python ./Scripts/Format/FormatFile.py Source/MechanicPic
echo -e "--- finish clang-format ---"
