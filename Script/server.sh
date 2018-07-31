#设置执行环境为当前脚本所在目录
cd `dirname $0`

ulimit -c unlimited
ulimit -n 32768


ARGS="IP=Any" 
ARGS=$ARGS" Port=9091"
ARGS=$ARGS" LQN=1000"
ARGS=$ARGS" ServerThreadSize=2"
ARGS=$ARGS" ClientHeartTime=60000000"
ARGS=$ARGS" ClientSendTime=200000"
ARGS=$ARGS" ClientRecvBufferSize=20480"
ARGS=$ARGS" ClientSendBufferSize=20480"

nohup ./main $ARGS >/dev/null 2>&1 &


read -p "..按任意键退出.." var
