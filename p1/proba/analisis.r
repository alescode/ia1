# R --slave --no-save --no-restore --no-environ --silent --args ejemplo.in-resultado.out < analisis.r 

generarMedidas <- function(vector) {
	rangoV = range(vector);
	rangoVector = rangoV[2]-rangoV[1];
	q1 = quantile(vector, 0.25)[[1]];
	q3 = quantile(vector, 0.75)[[1]];
	r = c(mean(vector), median(vector), rangoVector, sqrt(var(vector)), q3-q1);
	r
}

cat("-- reading arguments\n", sep = "");
cmd_args = commandArgs();

for (i in c(8:8)) {
    print(cmd_args[i]);
    Proporciones = read.table(cmd_args[i], header=F)[,1];
    print(generarMedidas(Proporciones));
    #Proporciones = Proporciones[-c(length(Proporciones))]; # Pop last
    hist(Proporciones, include.lowest=TRUE, right=TRUE, col="lime green", 
    main="Proporción de nodos comparados", xlab="Proporción", ylab="Frecuencia"
    , ylim = c(0, 1.2e6)
    );
    #lines(plot(Proporciones[,1]));  # Graficar lineas del histograma?
    #Boxplot
}
#Proporciones = read.table("", header=F)[,1];
