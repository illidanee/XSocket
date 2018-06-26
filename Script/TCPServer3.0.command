#设置执行环境为当前脚本所在目录
cd `dirname $0`

ip="any"
port=10000
lqn= 1000
./TCPServer3.0 $ip $port $lqn

read -p "..按任意键退出.." var
