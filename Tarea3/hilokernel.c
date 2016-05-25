/**
 * @author David Reyes
 * @description Programa que crea un hilo de kernel por medio de modulo. Para compilarlo se crea el archivo Makefile y se ejecuta 'make'. Para correrlo se ejecuta ingresando 'insmod hilokernel.ko'. Para detenerlo se ejecuta el comando 'rmmod hilokernel.ko'
 * @version 1.0
 * @date 24 May, 2016
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>

/** variable que se utilizará para guardar la referencia del hilo creado */
static struct task_struct *thread;

/**
 * @function hola
 * @description función que realizará el hilo, solo imprime un mensaje cada 5 segundos. En cada ciclo revisará si no hay una señar para detenerlo, en caso de haberla terminará el hilo
 */
static int hola(void *unsed){
	allow_signal(SIGKILL);
	while(!kthread_should_stop()){
		schedule();
		printk(KERN_INFO "Hilo corriendo\n");
		ssleep(5);
	}
	printk(KERN_INFO "Hilo detenido\n");
	return 0;
}

/**
 * @function init_thread
 * @description función para iniciar el hilo, se crea con la llamada thread_run()
 */
static int __init init_thread(void){
	printk(KERN_INFO "Creando hilo\n");
	thread = kthread_run(&hola,NULL,"khelloWorld");
	if(thread)
		printk(KERN_INFO "Hilo creado\n");
	else
		printk(KERN_ERR "Fallo en la creacion del hilo\n");
	return 0;
}

/**
 * @function cleanup_thread
 * @description función que se llama cuando se ejecuta 'rmmod', aqui detiene el hilo con la función kthread_stop
 */
static void __exit cleanup_thread(void){
	printk(KERN_INFO "limpiando\n");
	if(thread){
		kthread_stop(thread);
		printk(KERN_INFO "Hilo detenido");
	}
}

/** funcion que se ejecuta cuando se utiliza 'insmod', al hacerlo se va a la función init_thread */
module_init(init_thread);
/** funcion que se ejecuta cuando se utiliza 'rmmod', al hacerlo se va a la función cleanup_thread */
module_exit(cleanup_thread);
