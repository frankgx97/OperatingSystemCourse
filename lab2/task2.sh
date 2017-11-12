add()
{
    let result=$1+$2
    echo $1 "add" $2 "equals" $result 
}

substract()
{
    let result=$1-$2
    echo $1 "subtract" $2 "equals" $result  
}

divide()
{
    if [ $2 -eq 0 ];then
        echo "ERROR, divide by 0"
        return
    fi
    let result=$1/$2
    echo $1 "divide" $2 "equals" $result  
}

multiply()
{
    let result=$1*$2
    echo $1 "multiply" $2 "equals" $result  
}

remainder()
{
    if [ $2 -eq 0 ];then
        echo "ERROR, divide by 0"
        return
    fi
    let result=$1%$2
    echo $1 "remainder" $2 "equals" $result
}

while((1))
do
    echo "Caculator"
    echo "[a] Add"
    echo "[s] Subtract"
    echo "[m] Multiply"
    echo "[d] Divide"
    echo "[r] Remainder" 
    echo "[e] Exit"
    read -t 30 -p "Enter your choice:" ch
    if [ $ch = 'e' ];
    then
        exit 0
    fi

    read -p "Enter 2 numbers, seperate with a space:" a b
    case $ch in
        a)
            add $a $b
            ;;
        s)
            substract $a $b
            ;;
        m)
            multiply $a $b
            ;;
        d)
            divide $a $b
            ;;
        r)
            remainder $a $b
            ;;
        e)
            exit 0
            ;;
    esac
    echo '========================================='
done
