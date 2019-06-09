
/*
 Types declarations
 */

export type BaliseValue = boolean | number | string | null;
export type BaliseParameterValue = boolean | number | string | Buffer | null;
export type BaliseReturningBufferValue = boolean | number | Buffer | null;

export function setGlobalOption(name: string, value: string): void;

export class BaliseProcess {

    constructor();

    loadSourceCode(source: string): void;
    loadSourceFile(path: string): void;
    loadBinaryFile(path: string): void;

    setGlobalVariable(name: string, value: BaliseValue): void;
    getGlobalVariable(name: string): BaliseValue;

    executeFunction(name: string, ...parameters: BaliseParameterValue[]): BaliseValue;
    executeFunctionReturningBuffer(name: string, ...parameters: BaliseParameterValue[]): BaliseReturningBufferValue;

    executeFunctionParallel(name: string, parametersList: BaliseParameterValue[][]): BaliseValue[];
    killThreads(): void;
}

export interface ParallelOptions {
    debugMode?: boolean;
    waitReplyTimeout?: number;
    sleepTimeWhileBusy?: number;
    maxPhysicalMemoryUsage?: number;
}

export function createBaliseProcessParallel(threads: number, path: string, options?: ParallelOptions): BaliseProcess;
