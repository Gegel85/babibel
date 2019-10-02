local paths = { ... }
local codes = {
	"OK",
	"KO",
	"HELLO",
	"BYE",
	"CONNECT",
	"REGISTER",
	"GET_FRIENDS",
	"GET_USER_INFOS",
	"CALL",
	"CALL_REFUSED",
	"CALL_ACCEPTED"
}

function displayPacket(packet)
    op = string.byte(packet:sub(1, 1))
    key = packet:sub(6, 9)
    op = codes[(op ~ key:byte()) + 1]
    if not op then
        print("Invalid opcode")
    else
	print(op)
    end
    length = (packet:sub(2, 2):byte() << 24) + (packet:sub(3, 3):byte() << 16) + (packet:sub(4, 4):byte() << 8) + packet:sub(5, 5):byte()
    print("Length: ", length)
    print("Key:", (packet:sub(6, 6):byte() << 24) + (packet:sub(7, 7):byte() << 16) + (packet:sub(8, 8):byte() << 8) + packet:sub(9, 9):byte())
    data = {}
    for i = 1, length do
    	print(packet:sub(9 + i, 9 + i):byte() ~ key:sub(i % 4 + 1):byte())
    end
    return packet:sub(10 + length, #packet)
end

function displayPackets(path)
    local file = io.open(path, "rb")
    if not file then print("Cannot open "..path) return end
    local line = file:read()
    local packets = ""

    while line do
        packets = packets..line
        line = file:read()
        if line then
            packets = packets.."\n"
        end
    end
    file:close()
    while #packets ~= 0 do
        packets = displayPacket(packets)
    end
end

for i, k in pairs(paths) do
    print("Packets in "..k)
    displayPackets(k)
end