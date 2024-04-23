// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLOODBORN_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UButton* ResumeBtn;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UButton* RestartBtn;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UButton* QuitBtn;

private:

	UFUNCTION()
	void ResumeGame();

	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void QuitGame();

};
