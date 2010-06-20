# R --slave --no-save --no-restore --no-environ --silent --args ejemplo.in-resultado.out < analisis.r 

cat("-- reading arguments\n", sep = "");
cmd_args = commandArgs();

for (i in c(8:8)) {
    print(cmd_args[i]);
    Proporciones = read.table(cmd_args[i], header=F);
    #Proporciones = Proporciones[-c(length(Proporciones))]; # Pop last
    hist(Proporciones[,1], include.lowest=TRUE, right=TRUE, col="lime green", 
    labels=TRUE, main="Proporción de nodos comparados", xlab="Proporción", ylab="Frecuencia");
    #lines(plot(Proporciones[,1]));  # Graficar lineas del histograma?
    #Boxplot
}
#Proporciones = read.table("", header=F)[,1];
